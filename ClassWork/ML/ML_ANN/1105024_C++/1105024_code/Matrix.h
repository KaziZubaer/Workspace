class Matrix {
public:
    int dim[2],label;
    std::vector< std::vector<double> >mat;
    Matrix() {}
    Matrix(int d0,int d1) {
        std::vector<double>temp;
        int i;
        dim[0]=d0;
        dim[1]=d1;
        for(i=0;i<d1;i++) {
            temp.push_back(0);
        }
        for(i=0;i<d0;i++) {
            mat.push_back(temp);
        }
    }
    void SetDim(int d0,int d1) {
        std::vector<double>temp;
        int i;
        mat.clear();
        dim[0]=d0;
        dim[1]=d1;
        for(i=0;i<d1;i++) {
            temp.push_back(0);
        }
        for(i=0;i<d0;i++) {
            mat.push_back(temp);
        }
    }
    Matrix operator + (const Matrix &m) {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++) {
            for(j=0;j<dim[1];j++) {
                ret.mat[i][j]=mat[i][j]+m.mat[i][j];
            }
        }
	    return ret;
    }
    Matrix operator - (const Matrix &m) {
    	assert(dim[0]==m.dim[0]&&dim[1]==m.dim[1]);
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++) {
            for(j=0;j<dim[1];j++) {
                ret.mat[i][j]=mat[i][j]-m.mat[i][j];
            }
        }
	    return ret;
    }
    Matrix operator * (double c) {
    	Matrix ret=Matrix(dim[0],dim[1]);
    	int i,j;
	    for(i=0;i<dim[0];i++) {
            for(j=0;j<dim[1];j++) {
                ret.mat[i][j]=mat[i][j]*c;
            }
        }
	    return ret;
    }
    Matrix operator * (const Matrix &m) {
    	assert(dim[1]==m.dim[0]);
    	Matrix ret=Matrix(dim[0],m.dim[1]);
    	int i,j,k;
	    for(i=0;i<dim[0];i++) {
            for(j=0;j<m.dim[1];j++) {
                for(k=0;k<dim[1];k++) {
                    ret.mat[i][j]+=mat[i][k]*m.mat[k][j];
                }
            }
        }
	    return ret;
    }
    Matrix Transpose() {
    	Matrix ret=Matrix(dim[1],dim[0]);
    	int i,j;
	    for(i=0;i<dim[1];i++) {
            for(j=0;j<dim[0];j++) {
                ret.mat[i][j]=mat[j][i];
            }
        }
	    return ret;
    }
    void Show() {
        int i,j;
	    for(i=0;i<dim[0];i++) {
            for(j=0;j<dim[1];j++) {
                printf("%.7lf ",mat[i][j]);
            }
            printf("\n");
        }
    }
};
