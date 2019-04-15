
import numpy as np
from array import array
import networkx as nx
from jgraph import *
import matplotlib.pyplot as plt
#-------------------------------------------------------variables-----------------------------------------------------
N=14    #number of nodes
alpharate=0.3   #  0< alpha < 1
betarate=alpharate;
T=100
Qfinal=[0]*T
Qbest=-10
MVC=[0]*T
S = [0] * N
beta=[[-1 for col in range(N)] for row in range(T)]
edge=27

A=[  [0,1,1,1,1,0,0,0,0,0,0,0,0,0],[1,0,1,1,0,0,0,0,0,0,1,0,0,0],[1,1,0,1,0,0,0,0,0,0,0,0,0,0],
     [1,1,1,0,0,0,0,0,0,0,0,0,0,0],[1,0,0,0,0,1,1,1,1,0,0,0,0,0],[0,0,0,0,1,0,1,1,1,1,0,0,0,0],
     [0,0,0,0,0,1,0,1,1,0,0,0,0,0],[0,0,0,0,0,1,1,0,1,0,0,0,0,0],[0,0,0,0,1,1,1,1,0,0,0,0,0,0],
     [0,0,0,0,0,1,0,0,0,0,1,1,0,1] ,[0,1,0,0,0,0,0,0,0,1,0,0,1,1],[0,0,0,0,0,0,0,0,0,1,0,0,1,1],
     [0,0,0,0,0,0,0,0,0,0,1,1,0,1],[0,0,0,0,0,0,0,0,0,1,1,1,1,0]]

G = nx.from_numpy_matrix(np.array(A))
nx.draw(G)
plt.show()


Gf=G

#-----------------------------Neighbors----------------

a1 = array("i",[2,3,4,5])
a2=array("i",[1,3,4,11])
a3=array("i",[1,2,4])
a4=array("i",[1,2,3])
a5=array("i",[1,6,7,8,9])
a6=array("i",[5,7,8,9,10])
a7=array("i",[6,8,9])
a8=array("i",[6,7,9])
a9=array("i",[5,6,7,8])
a10=array("i",[6,11,12,14])
a11=array("i",[2,10,13,14])
a12=array("i",[10,13,14])
a13=array("i",[11,12,14])
a14=array("i",[10,11,12,13])

aaa=[np.array([a1]).T,np.array([a2]).T,np.array([a3]).T,np.array([a4]).T,np.array([a5]).T,np.array([a6]).T,np.array([a7]).T
   ,np.array([a8]).T,np.array([a9]).T,np.array([a10]).T,np.array([a11]).T,np.array([a12]).T,np.array([a13]).T,np.array([a14]).T]
print(aaa[7][0])
#--------------------------Node Degree--------------
r1=4
r2=4
r3=3
r4=3
r5=5
r6=5
r7=3
r8=3
r9=4
r10=4
r11=4
r12=3
r13=3
r14=4
r=[4,4,3,3,5,5,3,3,4,4,4,3,3,4]
#print('r i',r[0]);


#----------------------------------------------------------------


#--------------------------------Action probablity-------

p=[[0 for col in range(N)] for row in range(T)]

p[0][0]=[1/r1,1/r1,1/r1,1/r1]
p[0][1]=[1/r2,1/r2,1/r2,1/r2]
p[0][2]=[1/r3,1/r3,1/r3]
p[0][3]=[1/r4,1/r4,1/r4]
p[0][4]=[1/r5,1/r5,1/r5,1/r5,1/r5]
p[0][5]=[1/r6,1/r6,1/r6,1/r6,1/r6]
p[0][6]=[1/r7,1/r7,1/r7]
p[0][7]=[1/r8,1/r8,1/r8]
p[0][8]=[1/r9,1/r9,1/r9,1/r9]
p[0][9]=[1/r10,1/r10,1/r10,1/r10]
p[0][10]=[1/r11,1/r11,1/r11,1/r11]
p[0][11] = [1/r12,1/r12,1/r12]
p[0][12]=[1/r13,1/r13,1/r13]
p[0][13]=[1/r14,1/r14,1/r14,1/r14]
# print(p)
# print('pppppppp',p[0][0][0])
# M=0

for o in range (1,T):
    for n in range(0,N):
        p[o][n]=[-1]*r[n]
# print('p isssssssssssssssssssssssssssss',p)


w=[0 for col in range(N)]
for n in range(0,N):
    w[n]=[0]*r[n]
# print('wwwwwwwww',w)


z=[0 for col in range(N)]
for n in range(0,N):
    z[n]=[0.05]*r[n]
# print('zzzz',z)



d=[0 for col in range(N)]
for n in range(0,N):
    d[n]=[0]*r[n]
# print('dddd',d)

D=[0]*N
# print('D',D)
M=[[-1]*N for row in range(T)]
#----------------------------------------------------------Functions----------------------------------------

#return ACtion  for node i  according to p[i][ri]
def Action_selector(t,i,p):
    a0 = 0
    rand = np.random.uniform(0, 1)
    for m in range(0,r[i]):
         if (a0<= rand < a0 + p[t][i][m]):
            break
         else:
             a0=a0+p[t][i][m];
    return aaa[i][m][0], m


def decoder(S, u, mVC, currentComponent):
    mVC[u] = currentComponent
    if mVC[ S[u] ] == 0:
        mVC[u] = decoder(S, S[u], mVC, currentComponent)
    else:
        mVC[u] = mVC[S[u]]
    return mVC[u]


def Q(mVC,A,edge,r):
    temp = 0
    #print(mod[1])
    for p in range(0,N):
        for q in range(0,N):
            #print('q',q)
            delta=0;
            if(mVC[p]==mVC[q]):
                delta=1;
            temp= temp+ ((A[p][q] - r[p] * r[q]) / (2*edge)) * delta;

    Q=temp/(2*edge)
    # print('modularity is',Q)
    return Q;

def Eresponce(Q,Qbest,MVC):          # function to choose enviroment response
        for i in range(0,N):
            # check_dens = 0
            buf1 = MVC[t][i]
            c = 1
            cp = 0
            # temp_vec = [0;0];

            for k in range(N):
                if (A[i][k] == 1):
                    buf2 = MVC[t][k]
                    if (buf1 == buf2):
                        c = c + 1
                    else:
                        cp = cp + 1

            if( (Q>= Qbest) & (cp <= c) ):
                beta[t][i]=0;  #reward

            else:
                beta[t][i]=1;  #penalty
        # print('betaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa',beta[t])
        return beta[t];


def update_Qbest(Qbest,Qfinal):            # function for updating Q best according to env.response

    if(Qfinal[t]>= Qbest):
       Qbest=Qfinal[t];
    return Qbest;



def update_wzd(i,q,beta,w,z):                          #func of updating W and Z          #stiiiillll neeed work**************
    b=beta[t][i]
    # print('bbbb',b)
    rrr=r[i]
    for action in range(0,rrr):
        if ( q == action):
            # print('w', w[i][action])
            w[i][q]=w[i][q]+(1-b)
            # print('w', w[i][action])
            if(z[i][q]==0.05):
                z[i][q]=0
            z[i][q]=z[i][q]+1
            d[i][q] = w[i][q] / z[i][q]


    D[i]=d[i].index(max(d[i]))
    # print('D[i] is:', d[i])
    # print('D[i] is:',D[i])
    return w[i][q],z[i][q],D[i]; # q?action



def cprp_update(p,i,q):
    rrr=r[i]
    for action in range(rrr):
        if ( q == action):
            # print('p t i m',t,i,d,'is:',p[t][i][q])
            p[t+1][i][q]=float(p[t][i][q]+alpharate*(1-p[t][i][q]))

        if ( q != action):
            # print('2222 condision')
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

termcondition =0;
def teminationcondition(Qfinal,termcondition ):
    eps=0.5
    if(abs(Qfinal[-1] - Qfinal[-2]) > eps):
       termcondition=0
    else:
        termcondition =1+ termcondition ;
    if(termcondition>5):
        return 0
    else:
        return 1

t=0
flag=1


while (flag & t<T):

    S = [0] * N
    for i in range(N):
        S[i],m = Action_selector(t,i,p);
        M[t][i]=m
        # print('mmmmmmmmmmmmmmm',m)

    # print('solution Vector is:', S);

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
    # print("vector membership in time =",t,'is :',MVC[t])

    Qfinal[t]=Q(MVC[t], A,edge,r)
    beta[t]=Eresponce(Qfinal[t],Qbest,MVC)
    Qbest= update_Qbest(Qbest,Qfinal)
    # print('madularity is in t=:',t,'is   :',Qfinal[t])


    if(t>3):              #const Q
        falg = teminationcondition(Qfinal,termcondition)


    if(t==T-1):
         break
    else:
        for i in range (0,N):
            q=M[t][i]
            w[i][q], z[i][q],D[i] = update_wzd(i,q,beta,w,z)
            # D[i]=current_optimal_action(i,m,w,z)
            # p[t+1][i]=Lpr_updatep(p,i)
            p[t+1][i]=cprp_update(p,i,D[i])
            # print('p for node ',i,'is: ',p[t+1][i])

    t+=1


print('Qbest issssssssss:','        Qbest is',Qbest)
print('final resultttttttttt:',MVC[t],)
# for t in range (T):
#     if(Qfinal[t] == Qbest):
#         a=MVC[t]
#         print('final resultttttttttt:',a,)
#         break







#--------------------------------plot modularity---------------------------------------------------------------
# plt.plot([0,1,2,...,T], Qfinal)

# x_data = []*T
y_data = Qfinal
plt.plot( Qbest, 'rx')
plt.plot( y_data, 'b-')
plt.xlabel("Time (s)")
plt.ylabel("Modularity")

plt.show()

