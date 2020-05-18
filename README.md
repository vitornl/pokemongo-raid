# pokemongo-raid
Metaheuristic for finding the optimal route for raid events

the input files are in:
solver/data/*
where *_icarai are the files for the small instance and *_niteroi are the files for the full intance

To run all algorithms using the small instance just:
    make && ./main
to run the full instance have to change the LOCATION variable in main.cpp to 0, and then:
    make && ./main

the results presented on the technical report are in:
plotter/*

github: https://github.com/vitornl/pokemongo-raid