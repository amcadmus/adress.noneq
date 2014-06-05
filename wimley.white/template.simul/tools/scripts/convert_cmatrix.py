#!/usr/bin/env python

import numpy as np

from scipy.sparse import coo_matrix

import argparse

def load_cmatrix(filename):
    r"""Load EMMA1 matrix and return dense matrix."""
    rawdata=np.loadtxt(filename, skiprows=1)
    data=rawdata[:,2]
    row=rawdata[:,0].astype(int)
    col=rawdata[:, 1].astype(int)

    C=coo_matrix((data, (row, col))).toarray()
    return C

def tmatrix(C):
    r"""Compute transition matrix (non-reversible) from
    count-matrix."""
    T=C/C.sum(axis=1)[:,np.newaxis]
    return T


if __name__=="__main__":
    parser=argparse.ArgumentParser(description="Compute dense transition matrix"+\
                                       "in EMMA! dense format"+\
                                       "from EMMA1 sparse countmatrix")
    parser.add_argument("C", type=str, help="Input count matrix")
    parser.add_argument("outfile", type=str, help="Output transition matrix")
    args=parser.parse_args()
    C=load_cmatrix(args.C)
    T=tmatrix(C)
    M=T.shape[0]
    N=T.shape[1]    
    np.savetxt(args.outfile, T, header="DENSE "+str(M)+" "+str(N), comments='')

    
