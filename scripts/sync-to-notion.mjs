#!/usr/bin/env node
/**
 * For every LeetCode solution folder newly added in this push:
 *  1. Read the solution file (+ README.md if the extension made one)
 *  2. Ask an OpenRouter model for a structured explanation
 *  3. Create a page for it in a Notion database
 *
 * No npm dependencies required (Node 18+ has global fetch).
 */

import { readFileSync, existsSync, readdirSync } from "fs";
import path from "path";

const {
  OPENROUTER_API_KEY,
  NOTION_API_KEY,
  NOTION_DATABASE_ID,
  OPENROUTER_MODEL = "deepseek/deepseek-v4-flash-latest",
  GITHUB_REPOSITORY,
} = process.env;

const SOLUTION_EXTENSIONS = [".cpp", ".py", ".java", ".js", ".ts", ".go", ".c", ".cs", ".rb"];

function fail(msg) {
  console.error(`✖ ${msg}`);
  process.exit(1);
}

if (!OPENROUTER_API_KEY) fail("Missing OPENROUTER_API_KEY secret");
if (!NOTION_API_KEY) fail("Missing NOTION_API_KEY secret");
if (!NOTION_DATABASE_ID) fail("Missing NOTION_DATABASE_ID secret");

// ---------- 1. Which problem folders were added in this push? ----------
function getChangedFolders() {
  const diffPath = "diff.txt";
  if (!existsSync(diffPath)) fail("diff.txt not found — did the workflow's diff step run?");
  const lines = readFileSync(diffPath, "utf8").split("\n").filter(Boolean);

  const folders = new Set();
  for (const line of lines) {
    const [status, filePath] = line.split("\t");
    if (!filePath || status !== "A") continue; // only newly added files
    const ext = path.extname(filePath);
    const base = path.basename(filePath);
    if (base === "README.md") continue; // paired with the solution file, not the trigger
    if (!SOLUTION_EXTENSIONS.includes(ext)) continue;
    const folder = filePath.split("/")[0];
    if (folder && folder !== filePath) folders.add(folder);
  }
  return [...folders];
}

// ---------- 2. "0206-reverse-linked-list" -> { number: 206, title: "Reverse Linked List" } ----------
function parseFolderName(folder) {
  const match = folder.match(/^(\d+)-(.+)$/);
  const number = match ? Number(match[1]) : null;
  const slug = match ? match[2] : folder;
  const title = slug
    .split("-")
    .map((w) => w.charAt(0).toUpperCase() + w.slice(1))
    .join(" ");
  return { number, title };
}

// ---------- 3. Read the solution file + any README in the folder ----------
function readProblem(folder) {
  const files = readdirSync(folder);
  const solutionFile = files.find((f) => SOLUTION_EXTENSIONS.includes(path.extname(f)));
  if (!solutionFile) return null;

  const code = readFileSync(path.join(folder, solutionFile), "utf8");
  const language = path.extname(solutionFile).replace(".", "");

  let problemNotes = "";
  const readmePath = path.join(folder, "README.md");
  if (existsSync(readmePath)) problemNotes = readFileSync(readmePath, "utf8");

  return { code, language, problemNotes };
}

// ---------- 4. Ask OpenRouter for a structured explanation ----------
async function explain({ title, code, language, problemNotes }) {
  const prompt = `You are explaining a LeetCode solution for a personal study notebook.

Problem: ${title}

${problemNotes ? `Notes from the repo about this problem:\n${problemNotes}\n` : ""}
Solution (${language}):
\`\`\`${language}
${code}
\`\`\`

Return ONLY valid JSON (no markdown code fences, no commentary before or after) with this exact shape:
{
  "topics": ["1 to 4 short topic tags, e.g. Hash Table, Two Pointers"],
  "time_complexity": "e.g. O(n)",
  "space_complexity": "e.g. O(1)",
  "explanation_markdown": "A clear walkthrough in markdown using ## headings and bullet points: 1) restate the problem in 1-2 sentences, 2) explain the approach/intuition, 3) walk through the code step by step, 4) restate time/space complexity with a one-line reason."
}`;

  const res = await fetch("https://openrouter.ai/api/v1/chat/completions", {
    method: "POST",
    headers: {
      Authorization: `Bearer ${OPENROUTER_API_KEY}`,
      "Content-Type": "application/json",
      "HTTP-Referer": `https://github.com/${GITHUB_REPOSITORY || ""}`,
      "X-Title": "LeetCode Explainer",
    },
    body: JSON.stringify({
      model: OPENROUTER_MODEL,
      messages: [{ role: "user", content: prompt }],
      temperature: 0.3,
    }),
  });

  if (!res.ok) throw new Error(`OpenRouter error ${res.status}: ${await res.text()}`);

  const data = await res.json();
  const raw = data.choices?.[0]?.message?.content?.trim() ?? "";
  const cleaned = raw.replace(/^```json\s*|```$/g, "").trim();

  try {
    return JSON.parse(cleaned);
  } catch {
    throw new Error(`Could not parse model output as JSON:\n${raw}`);
  }
}

// ---------- 5. Tiny markdown -> Notion blocks converter ----------
function markdownToBlocks(markdown) {
  const blocks = [];
  const lines = (markdown || "").split("\n");
  let i = 0;

  while (i < lines.length) {
    const line = lines[i];

    if (line.startsWith("```")) {
      const lang = line.slice(3).trim() || "plain text";
      const codeLines = [];
      i++;
      while (i < lines.length && !lines[i].startsWith("```")) {
        codeLines.push(lines[i]);
        i++;
      }
      blocks.push({
        object: "block",
        type: "code",
        code: {
          rich_text: [{ type: "text", text: { content: codeLines.join("\n").slice(0, 2000) } }],
          language: mapLanguage(lang),
        },
      });
      i++;
      continue;
    }

    if (line.startsWith("## ")) blocks.push(heading(line.slice(3), 2));
    else if (line.startsWith("# ")) blocks.push(heading(line.slice(2), 1));
    else if (/^[-*]\s+/.test(line)) blocks.push(bullet(line.replace(/^[-*]\s+/, "")));
    else if (line.trim() !== "") blocks.push(paragraph(line));

    i++;
  }
  return blocks;
}

function heading(text, level) {
  const type = level === 1 ? "heading_1" : "heading_2";
  return { object: "block", type, [type]: { rich_text: [{ type: "text", text: { content: text.slice(0, 2000) } }] } };
}
function bullet(text) {
  return {
    object: "block",
    type: "bulleted_list_item",
    bulleted_list_item: { rich_text: [{ type: "text", text: { content: text.slice(0, 2000) } }] },
  };
}
function paragraph(text) {
  return { object: "block", type: "paragraph", paragraph: { rich_text: [{ type: "text", text: { content: text.slice(0, 2000) } }] } };
}
function mapLanguage(lang) {
  const known = ["javascript", "typescript", "python", "java", "c", "c++", "c#", "go", "ruby", "plain text"];
  const aliases = { js: "javascript", ts: "typescript", py: "python", cpp: "c++", cs: "c#" };
  const l = lang.toLowerCase();
  const resolved = aliases[l] || l;
  return known.includes(resolved) ? resolved : "plain text";
}

// ---------- 6. Create the Notion page ----------
async function createNotionPage({ title, number, language, folder, ai }) {
  const githubUrl = `https://github.com/${GITHUB_REPOSITORY}/tree/master/${folder}`;

  const properties = {
    Name: { title: [{ text: { content: title } }] },
    Language: { select: { name: language } },
    "GitHub Link": { url: githubUrl },
    Topics: { multi_select: (ai.topics || []).slice(0, 5).map((t) => ({ name: t })) },
    "Time Complexity": { rich_text: [{ text: { content: ai.time_complexity || "" } }] },
    "Space Complexity": { rich_text: [{ text: { content: ai.space_complexity || "" } }] },
    "Date Added": { date: { start: new Date().toISOString().split("T")[0] } },
  };
  if (number != null) properties["Problem Number"] = { number };

  const res = await fetch("https://api.notion.com/v1/pages", {
    method: "POST",
    headers: {
      Authorization: `Bearer ${NOTION_API_KEY}`,
      "Content-Type": "application/json",
      "Notion-Version": "2022-06-28",
    },
    body: JSON.stringify({
      parent: { database_id: NOTION_DATABASE_ID },
      properties,
      children: markdownToBlocks(ai.explanation_markdown),
    }),
  });

  if (!res.ok) throw new Error(`Notion error ${res.status}: ${await res.text()}`);
  return res.json();
}

// ---------- main ----------
async function main() {
  const folders = getChangedFolders();
  if (folders.length === 0) {
    console.log("No new solution folders in this push. Nothing to do.");
    return;
  }

  for (const folder of folders) {
    console.log(`\n→ Processing ${folder}`);
    const problem = readProblem(folder);
    if (!problem) {
      console.log("  (skipped — no recognizable solution file)");
      continue;
    }
    const { number, title } = parseFolderName(folder);

    try {
      const ai = await explain({ title, code: problem.code, language: problem.language, problemNotes: problem.problemNotes });
      const page = await createNotionPage({ title, number, language: problem.language, folder, ai });
      console.log(`  ✓ Created Notion page: ${page.url}`);
    } catch (err) {
      console.error(`  ✖ Failed on ${folder}: ${err.message}`);
      process.exitCode = 1;
    }
  }
}

main();
