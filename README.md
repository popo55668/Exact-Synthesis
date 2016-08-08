Exact Synthesis
====
Package for generating minimum-size 4-input majority-inverter graphs (MIGs)

For the implementation details, please refer to [Optimizing Majority-Inverter Graphs With Functional Hashing](http://msoeken.github.io/papers/2016_date_3.pdf)

Installation
====

Dependency Installation
----
Install [Satisfiability Module Theory (SMT) Z3](https://github.com/Z3Prover/z3)

    git clone https://github.com/Z3Prover/z3.git
    cd z3
    python scripts/mk_make.py
    cd build
    make
    sudo make install

Compilation
----
    git clone https://github.com/popo55668/exact-synthesis.git
    cd exact-synthesis
    make
