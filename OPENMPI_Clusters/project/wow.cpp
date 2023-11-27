double *solveEvaluation(int **dimensions, double **arrays, string evaluation)
{
    stack<double **> stk;
    stack<int *> dimensionsStack;

    for (int i = 0; i < evaluation.length(); i++)
    {
        if (isdigit(evaluation[i]))
        {
            // Push the matrix onto the stack
            stk.push(arrays[evaluation[i] - '1']);
            // Push the dimensions onto the stack
            dimensionsStack.push(dimensions[evaluation[i] - '1']);
        }
        else if (evaluation[i] == ')')
        {
            // Pop the top two matrices, multiply, and push the result back
            double **matrix2 = stk.top();
            stk.pop();
            int *dim2 = dimensionsStack.top();
            dimensionsStack.pop();

            double **matrix1 = stk.top();
            stk.pop();
            int *dim1 = dimensionsStack.top();
            dimensionsStack.pop();

            int r1 = dim1[0];
            int c1 = dim1[1];
            int c2 = dim2[1];

            double **result = new double *[r1];
            for (int i = 0; i < r1; ++i)
                result[i] = new double[c2];

            //multiply(matrix1, matrix2, result, r1, c1, c2);

            // Push the result onto the stack
            stk.push(result);

            int *resultDim = new int[2];
            resultDim[0] = r1;
            resultDim[1] = c2;
            dimensionsStack.push(resultDim);
        }
    }

    // The final result should be at the top of the stack
    return stk.top();
}