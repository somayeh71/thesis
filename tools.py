from array import array
import pandas as pd
import numpy as np
#-----------------------------Neighborsfinder----------------

def nodfinder(A,i,m,N):
    count=0
    for act in range(N):
        if (A[i][act] == 1):
            if(count==m):
                s=act+1
                return s
            count = count + 1

def getData(a,*args):
    if str(args) != '()':
        b = args[0]
        result=pd.read_csv(b)
        result=result.values[:,1]
        data = pd.read_csv(a)
        A = data.values.tolist()
        return A, result
    else:
        data = pd.read_csv(a)
        A = data.values.tolist()
        return A


#return ACtion  for node i  according to p[i][ri]
def Action_selector(t,i,p,A,r, N):
    s=0
    a0 = 0
    rand = np.random.uniform(0, 1)
    for m in range(0,r[i]):
         if (a0<= rand < a0 + p[t][i][m]):
            break
         else:
             a0=a0+p[t][i][m]
             s=nodfinder(A,i,m,N)
    return s, m

def edgeounter(r,N):          #find number of edges
    degree=0
    for i in range(N):
        degree=r[i]+degree
    return (degree/2)

def decoder(S, u, mVC, currentComponent):
    mVC[u] = currentComponent
    if mVC[ S[u] ] == 0:
        mVC[u] = decoder(S, S[u], mVC, currentComponent)
    else:
        mVC[u] = mVC[S[u]]
    return mVC[u]


def Q(mVC,A,edge,r,N):
    temp = 0
    for p in range(0,N):
        for q in range(0,N):
            if(mVC[p]==mVC[q]):
                temp= temp +( A[p][q] -( (r[p] * r[q]) / (2*edge)))

    Q=temp/(2*edge)
    return Q

def Eresponce(Q,Qbest,MVC,N,t,A,beta):
        for i in range(0,N):
            buf1 = MVC[t][i]
            c = 1            #raghavan
            cp = 0

            for k in range(N):
                if (A[i][k] == 1):
                    buf2 = MVC[t][k]
                    if (buf1 == buf2):
                        c = c + 1
                    else:
                        cp = cp + 1

            if( (Q>= Qbest) & (cp <= c) ):
                beta[t][i]=0        #reward

            else:
                beta[t][i]=1        #penalty
        return beta[t]


def update_Qbest(Qbest,Qfinal,t):            # function for updating Q best according to env.response

    if(Qfinal[t]> Qbest):
       Qbest=Qfinal[t]
    return Qbest



def update_wzd(i,q,beta,w,z,t,D,r,d):                          #func of updating W and Z          #stiiiillll neeed work**************
    b=beta[t][i]
    rrr=r[i]
    for action in range(0,rrr):
        if ( q == action):
            w[i][q]=w[i][q]+(1-b)
            if(z[i][q]==0.05):
                z[i][q]=0
            z[i][q]=z[i][q]+1
            d[i][q] = w[i][q] / z[i][q]


    D[i]=d[i].index(max(d[i]))
    return w[i][q],z[i][q],D[i]; # q?action


def cprp_update(p,i,q,t,alpharate,r):
    rrr=r[i]
    for action in range(rrr):
        if ( q == action):
            p[t+1][i][q]=float(p[t][i][q]+alpharate*(1-p[t][i][q]))

        if ( q != action):
            p[t + 1][i][action] = float((1 - alpharate) * p[t][i][action])
    return p[t+1][i]


# def Lpr_updatep(p,i):   #workkkkkkkkkkkk on this
#     rrr=r[i]
#               #M=1 mm=9
#     for action in range(rrr):
#         if ( beta[t][i]==0 & M == action):
#             # print('1111 condision')
#             print("beta isss",beta[t][i])
#             print('p t i m',t,i,M,'is:',p[t][i][M])
#             p[t+1][i][M]=float(p[t][i][M]+alpharate*(1-p[t][i][M]))
#
#             # print('p[t+1][i][action]:', p[t + 1][i][action])
#
#         if( beta[t][i]==0 & M!=action):
#             # print('2222 condision')
#             p[t+1][i][action]=float((1-alpharate)*p[t][i][action])
#
#         #-----------------------------PENALTY UPDTE
#
#         if (beta[t][i]==1 & action== M):
#             # print('item',(1 - betarate) * p[t][i][M])
#             p[t + 1][i][M] = float((1 - betarate) * p[t][i][M])
#             # print('3333 condision')
#
#         if(beta[t][i]==1 & action!= M):
#             # print('444 condision')
#             # print('p[t][i][action]:',t,i,p[t][i][action])
#             # print('item',(betarate/(rrr-1))+(1-betarate)*(p[t][i][action]))
#             p[t+1][i][action]=float((betarate/(rrr-1))+(1-betarate)*(p[t][i][action]))
#             # print('p[t+1][i][action]:', p[t+1][i][action])
#
#     # print('p is:',t+1,'for node i',i ,p[t+1][i])
#     return p[t+1][i]


def teminationcondition1(Qfinal,term1,t ):
    if(abs(Qfinal[t] - Qfinal[t-1])!= 0):
        term1=0
        return 1, term1
    else:
        term1 =1+ term1
        if( term1 >= 2000 ):
            return 0,term1
        else:
            return 1,term1


def deltacounter(MVC,N,t):
    delta=0
    for i in range(N):
        # print('mvc t n',MVC[t])
        # print('mvc t-1 n', MVC[t-1])
        if(MVC[t][i]!=MVC[t-1][i]):
            # print('in loop',MVC[t][i],MVC[t-1][i])
            delta=delta+1
            # print('detaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa is',delta)
    return delta

def teminationcondition2(Delta,term2,t ):

    if(Delta[t]!= 0):
        term2=0
        #print('delta is changing',Delta[t])           #uncomment later
        return 1, term2
    else:
        term2 =1+ term2
        if( term2 >= 100 ):
            return 0,term2
        else:
            return 1,term2




#-------------------------------------------------end of functions-----------------------

