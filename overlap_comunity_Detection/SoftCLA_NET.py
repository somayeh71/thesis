
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from sklearn.metrics.cluster import normalized_mutual_info_score
from tools import *
from sklearn import metrics

#-------------------------------------------------------variables-----------------------------------------------------
#G = nx.from_numpy_matrix(np.array(A))
#nx.draw(G)
#Gf=G
#--------------------------------------------------------------------------------------------------------------

def CLA_NET(a,*args):

    if str(args) != '()':
        b = args[0]
        A,result=getData(a,b)
    elif str(args) == '()':
        A = getData(a)
    print('Data loaded.')


    #----------------initial values-------------------------------------------------------------------
    N=len(A)  #number of nodes
    alpharate=0.07   #  0< alpha < 1
    # betarate=alpharate;
    T=100000
    Qfinal=[0]*T
    Delta=[0]*T
    Qbest= -10
    MVC=[0]*T
    beta=[[-1 for col in range(N)] for row in range(T)]
    #-------------------------- Degree------------------------------------------------

    r = np.zeros(N)
    r = [ int(x) for x in r ]     #convert all elements to int from float

    for row in range( N):
        for col in range(N):
            if(A[row][col]==1):
                r[row]=r[row]+1

    edge = edgeounter(r,N)
    # print('number od edges',edge)
    #-----------------------------------

    #--------------------------------Action probablity-------

    p=[[0 for col in range(N)] for row in range(T)]

    for i in range(0,N):
        if(r[i] == 0):    #isolated nodes
            p[0][i] = 0
        else:
            p[0][i] = [1 / r[i]] * r[i]

    for o in range (1,T):
        for n in range(0,N):
            p[o][n]=[-1]*r[n]


    w=[0 for col in range(N)]
    for n in range(0,N):
        w[n]=[0]*r[n]

    z=[0 for col in range(N)]
    for n in range(0,N):
        z[n]=[0]*r[n] # be careful ----maybe non zero amount is nice

    d=[0 for col in range(N)]
    for n in range(0,N):
        d[n]=[0]*r[n]        #0


    D=[0]*N

    M=[[0]*N for row in range(T)]




    term1 =0
    term2=0
    t=0
    flag=1

    while (flag and t<T):
        #print('counting iteration:',t)          #uncommetnt later
        S = [0] * N

        for i in range(N):
            S[i], M[t][i] = Action_selector(t,i,p,A,r,N)



        S = [0] + list(S)  # to handle the 1-indexing of the content in S
        mVC = [0] * len(S)
        currentComponent = 1

        for i in range(1, len(S)):
            if mVC[i] == 0:
                componentAssigned = decoder(S, i, mVC, currentComponent)
                if componentAssigned == currentComponent:
                    currentComponent += 1
        mVC = mVC[1:]  # Gets rid of the dummy 0th element added above

        MVC[t]=mVC
        Qfinal[t]=Q(MVC[t], A,edge,r,N)
        beta[t]=Eresponce(Qfinal[t],Qbest,MVC,N,t,A,beta)
        Qbest= update_Qbest(Qbest,Qfinal,t)

        for i in range(0, N):
            m = M[t][i]  # be careful
            print('current action', m)
            w[i],z[i] = update_wzd(i, m, beta, w, z, t, D, r, d)

            d[i] = np.divide( w[i] , z[i])

            D[i] = d[i].index( max( d[i] ))
            # p[t+1][i]=Lpr_updatep(p,i)
            # p[t+1][i]=cprp_update(p,i,D[i],t,alpharate,r)
            p[t + 1][i] = cprp_update(p, i, D[i], t, alpharate, r)


        if(t==T-1):
             break
        else:
            for i in range (N):
                m = M[t][i] #be careful
                print('current action',m)
                D[i] = update_wzd(i,m,beta,w,z,t,D,r,d)

                # p[t+1][i]=Lpr_updatep(p,i)
                # p[t+1][i]=cprp_update(p,i,D[i],t,alpharate,r)
                p[t + 1][i] = cprp_update(p, i, D[i], t, alpharate, r)


        if(t>3 ):
            Delta[t] =deltacounter(MVC,N,t)
            # print('dddddddddddddd',Delta)
            flag1,term1 = teminationcondition1(Qfinal,term1,t) #const Q
            flag2, term2 = teminationcondition2(Delta,term2,t )  # const Mvc

            if(flag1==0 or flag2==0):
                flag=0
            if(flag==0):
                #print('stoped in iteration :',t)
                break

        t+=1

    #print('final resultttttttttt:',MVC[t],)
    #print('labels',result)
    # for t in range (T):
    #     if(Qfinal[t] == Qbest):
    #         a=MVC[t]
    #         tbest=t
    #         print('final resultttttttttt:',a,)
    #         print('t best :',t)
    #         break




    print("-----------------------------------------------------------------")
    print('Number of nodes: ',N )
    print('Number of edges:',edge)
    print('stoped in iteration :',t)
    print('Qfinal issssssssss:', Qfinal[t])
    #print(' MVC:  ', MVC[t])
    lablematrix= lablematrixcreator( MVC[t])
   # print('lable matrix:',lablematrix)
    ov = 2
    softm = softm(MVC)
    print('Soft comunity detection is:', softm)

    if str(args) != '()':
        nmi = normalized_mutual_info_score(result, MVC[t])
        print('NMI is: ', nmi)
        purity = purity_score(result, MVC[t])
        print('Purity is:', purity)
    print('lable size is',len(MVC[t]))

    #silhouette = metrics.silhouette_score(A, lablematrix, metric='sqeuclidean')
    silhouette = metrics.silhouette_score(A,MVC[t] , metric='sqeuclidean')

    print("Silhouette Coefficient: ",silhouette )
    #--------------------------------plot modularity---------------------------------------------------------------
    # plt.plot([0,1,2,...,T], Qfinal)

    #x_data = []*T

    #*****************baaresi ravande hamgeraiiiii p ha
    # for i in range(T):
    #     for j in range(N):
    #         print(p[i][j][0])
    #     print('/n')





    y_data = Qfinal
    # y_data=[0]*t
    plt.subplot(2, 1, 1)
    plt.plot( Qbest, 'rx')
    plt.plot( y_data[0:t], 'b-')
    # plt.xlabel("Iteration")
    plt.ylabel("Modularity")

    plt.subplot(2, 1, 2)
    plt.plot( Delta[0:t], 'r-')
    plt.xlabel("Iteration")
    plt.ylabel("Delta")



    #dddd=p[:][0][:]
    #print('d is',dddd)
    #print ('p size is:',np.size(p),'shape is:', np.shape(p))
    #plt.plot(p[:][0][:],'r-')


    plt.show()
    if str(args) != '()':
        return edge,Qfinal[t], t, MVC[t],silhouette,lablematrix,nmi,purity

    return edge,Qfinal[t],t, MVC[t],silhouette,lablematrix









# a="siz0010.csv"
# b="siz0010-membership.csv"
# CLA_NET(a,b)








#