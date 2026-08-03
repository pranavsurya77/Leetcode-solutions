class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        //coloumn -> matrix[0][...]
        //row -> matrix[...][0]
        int rows=matrix.size();
        int cols=matrix[0].size();
        int col0=1;

        //mark the 1st row and col to be zero
        for(int i=0;i<rows;i++){
            for(int j=0;j<cols;j++){
                if(matrix[i][j]==0){
                    matrix[i][0]=0;
                    if(j!=0){
                    matrix[0][j]=0;
                    }else{
                        col0=0;
                    }
                }
            }
        }

        //iteration of the smaller box
        for(int i=1;i<rows;i++){
            for(int j=1;j<cols;j++){
                if(matrix[i][0]==0 || matrix[0][j]==0){
                    matrix[i][j]=0;
                }
            }
        }

        //then finallly check the first row and col itself
        //they only depend on the col0 variable
        if(matrix[0][0]==0){
            for(int j=0;j<cols;j++) matrix[0][j]=0;
        }
        if(col0==0){
            for(int i=0;i<rows;i++) matrix[i][0]=0;
        }
    }
};