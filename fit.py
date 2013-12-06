import pylab
adc=[2171716946,2146230895,1957384498,1685286743,1445450033,1193597004,1190296037,1114721776,1036572982,989725036,969918591,951013100,944840660,942224072,938277455,938088937]
v=[]
for i in adc:
    v.append(i/1000000.0)
L=[15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90]
V=[]
for i in range(len(v)):
    V.append(1/v[i])
pylab.plot(V,L,'g-')
#pylab.plot(L,V,'r')
a,b,c,d = pylab.polyfit(V,L,3)
fit = []
for i in range(len(v)):
    fit.append(a*V[i]**3+b*V[i]**2+c*V[i]**1+d)
pylab.plot(V,fit,'b')

print "R="+str(a)+"*V^3+"+str(b)+"*V^2+"+str(c)+"*V^1+"+str(d)

pylab.show()
