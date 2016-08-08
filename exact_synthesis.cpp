/****************************************************************************
  FileName     [ exact_synthesis.cpp ]
  PackageName  [ MIG ]
  Synopsis     [ Generate 4-input subgraphs ]
  Author       [ Li-Wei Wang ]
****************************************************************************/

#include <vector>
#include <fstream>
#include "z3++.h"

using namespace z3;
using namespace std;

const unsigned VarToClassNum[] = { 0, 2, 4, 14, 222, 616126 };
const unsigned ClassToNodeNum[] = 
{
    0, 3, 2, 3, 3, 1, 4, 2, 4, 4, 3, 4, 3, 3, 4, 2, 4, 4, 3, 4, 
    3, 0, 6, 4, 5, 4, 4, 3, 5, 2, 5, 4, 3, 5, 4, 5, 3, 5, 4, 5, 
    4, 5, 4, 5, 2, 5, 5, 5, 5, 5, 5, 5, 4, 4, 6, 6, 5, 5, 5, 4, 
    5, 4, 5, 5, 5, 4, 6, 5, 5, 4, 5, 5, 5, 3, 5, 5, 5, 5, 5, 4, 
    5, 4, 5, 1, 5, 3, 6, 5, 5, 4, 6, 3, 6, 5, 5, 5, 5, 5, 5, 4, 
    5, 5, 5, 4, 5, 4, 5, 5, 5, 3, 5, 5, 6, 4, 6, 6, 5, 5, 5, 5, 
    4, 5, 5, 5, 5, 4, 4, 6, 6, 4, 5, 5, 5, 5, 5, 6, 5, 5, 4, 5, 
    5, 4, 5, 3, 5, 5, 5, 6, 5, 6, 5, 5, 5, 5, 5, 5, 5, 4, 5, 5, 
    5, 5, 5, 5, 5, 5, 6, 5, 5, 6, 5, 5, 6, 5, 5, 5, 5, 4, 5, 5, 
    3, 6, 7, 6, 6, 6, 6, 6, 5, 5, 5, 6, 6, 5, 5, 4, 5, 5, 6, 5, 
    5, 6, 6, 5, 5, 6, 6, 4, 5, 6, 5, 5, 4, 4, 6, 6, 5, 5, 5, 4, 
    3, 6
};

const unsigned ClassToTruth[] = 
{
    0x0000, 0x0001, 0x0003, 0x0006, 0x0007, 0x000F, 0x0016, 0x0017, 0x0018, 0x0019, 
    0x001B, 0x001E, 0x001F, 0x003C, 0x003D, 0x003F, 0x0069, 0x006B, 0x006F, 0x007E, 
    0x007F, 0x00FF, 0x0116, 0x0117, 0x0118, 0x0119, 0x011A, 0x011B, 0x011E, 0x011F, 
    0x012C, 0x012D, 0x012F, 0x013C, 0x013D, 0x013E, 0x013F, 0x0168, 0x0169, 0x016A, 
    0x016B, 0x016E, 0x016F, 0x017E, 0x017F, 0x0180, 0x0181, 0x0182, 0x0183, 0x0186, 
    0x0187, 0x0189, 0x018B, 0x018F, 0x0196, 0x0197, 0x0198, 0x0199, 0x019A, 0x019B, 
    0x019E, 0x019F, 0x01A8, 0x01A9, 0x01AA, 0x01AB, 0x01AC, 0x01AD, 0x01AE, 0x01AF, 
    0x01BC, 0x01BD, 0x01BE, 0x01BF, 0x01E8, 0x01E9, 0x01EA, 0x01EB, 0x01EE, 0x01EF, 
    0x01FE, 0x033C, 0x033D, 0x033F, 0x0356, 0x0357, 0x0358, 0x0359, 0x035A, 0x035B, 
    0x035E, 0x035F, 0x0368, 0x0369, 0x036A, 0x036B, 0x036C, 0x036D, 0x036E, 0x036F, 
    0x037C, 0x037D, 0x037E, 0x03C0, 0x03C1, 0x03C3, 0x03C5, 0x03C6, 0x03C7, 0x03CF, 
    0x03D4, 0x03D5, 0x03D6, 0x03D7, 0x03D8, 0x03D9, 0x03DB, 0x03DC, 0x03DD, 0x03DE, 
    0x03FC, 0x0660, 0x0661, 0x0662, 0x0663, 0x0666, 0x0667, 0x0669, 0x066B, 0x066F, 
    0x0672, 0x0673, 0x0676, 0x0678, 0x0679, 0x067A, 0x067B, 0x067E, 0x0690, 0x0691, 
    0x0693, 0x0696, 0x0697, 0x069F, 0x06B0, 0x06B1, 0x06B2, 0x06B3, 0x06B4, 0x06B5, 
    0x06B6, 0x06B7, 0x06B9, 0x06BD, 0x06F0, 0x06F1, 0x06F2, 0x06F6, 0x06F9, 0x0776, 
    0x0778, 0x0779, 0x077A, 0x077E, 0x07B0, 0x07B1, 0x07B4, 0x07B5, 0x07B6, 0x07BC, 
    0x07E0, 0x07E1, 0x07E2, 0x07E3, 0x07E6, 0x07E9, 0x07F0, 0x07F1, 0x07F2, 0x07F8, 
    0x0FF0, 0x1668, 0x1669, 0x166A, 0x166B, 0x166E, 0x167E, 0x1681, 0x1683, 0x1686, 
    0x1687, 0x1689, 0x168B, 0x168E, 0x1696, 0x1697, 0x1698, 0x1699, 0x169A, 0x169B, 
    0x169E, 0x16A9, 0x16AC, 0x16AD, 0x16BC, 0x16E9, 0x177E, 0x178E, 0x1796, 0x1798, 
    0x179A, 0x17AC, 0x17E8, 0x18E7, 0x19E1, 0x19E3, 0x19E6, 0x1BD8, 0x1BE4, 0x1EE1, 
    0x3CC3, 0x6996
};

const int VAR = 4;
const int FUNC = 1 << VAR;
const int NPN = VarToClassNum[VAR];
context ctx;

class Node {
    public:
        Node(int i): id(i), b(ctx), a1(ctx), a2(ctx), a3(ctx), ss(ctx), ps(ctx) 
        {
            initVars();
        }

        void initVars() 
        {
            for (int i = 0; i < FUNC; ++i) {
                stringstream name;  name << "b_" << id << "_" << i;
                b.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < FUNC; ++i) {
                stringstream name;  name << "a1_" << id << "_" << i;
                a1.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < FUNC; ++i) {
                stringstream name;  name << "a2_" << id << "_" << i;
                a2.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < FUNC; ++i) {
                stringstream name;  name << "a3_" << id << "_" << i;
                a3.push_back(ctx.bool_const(name.str().c_str()));
            }
            for (int i = 0; i < 3; ++i) {
                stringstream name;  name << "s_" << i << "_" << id;
                ss.push_back(ctx.int_const(name.str().c_str()));
            }
            for (int i = 0; i < 3; ++i) {
                stringstream name;  name << "p_" << i << "_" << id;
                ps.push_back(ctx.bool_const(name.str().c_str()));
            }
        }

        // Majority functionality
        void addMajorityFormula(solver& svr)
        {
            for (int i = 0; i < FUNC; ++i) {
                expr bb = b[i], aa1 = a1[i], aa2 = a2[i], aa3 = a3[i];
                expr f = ( bb == ((aa1&&aa2)||(aa2&&aa3)||(aa1&&aa3)) );
                svr.add(f);
            }
        }

        // Input connection: 1.constant  2.variables
        void addConnectionFormula(solver& svr)
        {
            expr_vector* aa;
            for (int i = 0; i < 3; ++i) {
                expr ssi = ss[i];
                svr.add( (ssi < VAR + id) );  
                svr.add( (ssi >= 0) );

                if (i == 0) aa = &a1;
                else if (i == 1) aa = &a2;
                else aa = &a3;
                
                // constant
                for (int j = 0; j < FUNC; ++j) {
                    expr impl = implies( (ssi == 0), ((*aa)[j] == ps[i]) );
                    svr.add(impl);
                }
                // variables
                for (int j = 1; j <= VAR; ++j) {
                    for (int k = 0; k < FUNC; ++k) {
                        bool value = (k >> (j-1)) & 01;
                        expr psi = ps[i];
                        expr impl = implies( (ssi == j), ((*aa)[k] == ((value&&!psi)||(!value&&psi))) );
                        svr.add(impl);
                    }
                }
            }
        }

        // Symmetry breaking
        void addSymmetryFormula(solver& svr)
        {
            svr.add( (ss[0] < ss[1]) );
            svr.add( (ss[1] < ss[2]) );
        }

        int             id;
        expr_vector      b;
        expr_vector     a1;
        expr_vector     a2;
        expr_vector     a3;
        expr_vector     ss;
        expr_vector     ps;
};

int main(int argv, char** argc) {
    unsigned SUBGRAPH_BOUND = 5;
    for (int c = 0; c < NPN; ++c) {
        unsigned uTruth = ClassToTruth[c];
        unsigned nNode = ClassToNodeNum[c];
        cout << "Generating subgraphs for class " << c << ",";
        cout << " truth = ";  printf("0x%04X,", uTruth);
        cout << " min k = " << nNode << endl;
        if (nNode == 0) continue;

        vector<Node> nodes;
        for (int i = 1; i <= nNode; ++i) nodes.push_back(Node(i)); 

        solver svr(ctx);
        for (int i = 0; i < nNode; ++i) {
            nodes[i].addMajorityFormula(svr);
            nodes[i].addConnectionFormula(svr);
            nodes[i].addSymmetryFormula(svr);
        }

        // Input connection: majority node 
        for (int i = 0; i < nNode; ++i) {
            Node* maj = &nodes[i];
            for (int j = 1; j < maj->id; ++j) {
                Node* in = &nodes[j-1]; // topologically lower nodes
                expr_vector* aa;
                for (int k = 0; k < 3; ++k) {
                    if (k == 0) aa = &maj->a1;
                    else if (k == 1) aa = &maj->a2;
                    else aa = &maj->a3;

                    expr ssk = maj->ss[k];
                    expr psk = maj->ps[k];
                    for (int l = 0; l < FUNC; ++l) {
                        expr bl = in->b[l];
                        expr impl = implies( (ssk == VAR + j), ((*aa)[l] == ((bl&&!psk)||(!bl&&psk))) );
                        svr.add(impl);
                    }
                }
            }
        }

        // Function Semantics
        expr root_p = ctx.bool_const("root_p");
        Node* root_node = &nodes.back();
        for (int i = 0; i < FUNC; ++i) {
            bool value = (uTruth >> i) & 01;
            expr bi = root_node->b[i];
            expr f = (bi == ((root_p && !value) || (!root_p && value)));
            svr.add(f);
        }

        // In our implementation, output edge should not be negated.
        svr.add(!root_p);

        // Subgraphs generation
        ofstream out("temp.txt", ios::out);
        unsigned nGraph = 0;
        while (svr.check() && nGraph < SUBGRAPH_BOUND) {
            nGraph++;
            model mod = svr.get_model();
            expr block = (root_p == mod.eval(root_p));
            out << nodes.size() << endl;
            for (int i = 0; i < nodes.size(); ++i) {
                for (int j = 0; j < 3; ++j) {
                    out << mod.eval(nodes[i].ss[j]) << " " << mod.eval(nodes[i].ps[j]) << " ";
                    block = block && (nodes[i].ss[j] == mod.eval(nodes[i].ss[j]));
                    //block = block && (nodes[i].ps[j] == mod.eval(nodes[i].ps[j]));
                }
                out << "\n";
            }
            out << mod.eval(root_p) << endl << endl;
            svr.add(!block);
        }
        out.close();
        ifstream ifile("temp.txt", ios::in);
        stringstream str;  str << c << ".net";
        ofstream ofile(str.str().c_str(), ios::out | ios::app );
        ofile << nGraph << endl;
        ofile << ifile.rdbuf();
        ofile.close();  ifile.close();
    }
    return 0;
}
