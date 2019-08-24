library(igraph)
library(NMI)
## Two groups with not only few connection between groups
#pm <- cbind( c(.1, .001), c(.001, .05) )
#g <- sample_sbm(1000, pref.matrix=pm, block.sizes=c(300,700))

s=0.005
p<-cbind( c(1-s, s,0), c(s, 1-s,0),c(0, 0,1)) 
n=75
my_block_sizes <- c(30,25,20)


g <- sample_sbm(n, pref.matrix=p, my_block_sizes )

plot( g)
membership_vector=rep(seq_along(my_block_sizes), my_block_sizes)
set.seed(1) 
rand <- sample(10)
rand

membership_vector=t(t(membership_vector))

wc <- cluster_walktrap(g)


modularity(wc)
a=membership(wc)

plot(wc, g)
#k<-[1,5,6,11,12,13,15,16],[2,3,4,7,8,9,10,14,17],[18,19,20,21]
plot(k ,g)
plot
#nmi in r
membership_vector = cbind(c(1:n), membership_vector)
a = cbind(c(1:n), a)
NMI(membership_vector, a)
plot(a, g)






### fastgreedy.community
fc <- fastgreedy.community(g)
modularity(fc)
fa=membership(fc)
plot(fc, g)
#f = cbind(c(1:n), f)
NMI(membership_vector, f)



#save matrix for python
mat=as_adjacency_matrix(g)
write.csv(as.data.frame.matrix(mat),'sizee1000.csv',row.names = F)
mat

#save membership vector for python
write.csv(as.data.frame.matrix(membership_vector),'sizee1000-membership.csv',row.names = F, )

membership_vector

