Tensor<int, 1> A({10});
{
    for(int i=0; i<10; i++) {
        auto init_j = 0;
        for(int j=0; j<20; j++) {
            init_j = init_j + B.at({i, j}) * C.at({j});
        }
        A.at({i}) = init_j;
    }
}
