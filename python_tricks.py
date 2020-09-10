def isPrime(n): 
    if(n<=1):
        return 0
    if(n<=3):
        return 1  
    if (n%2==0 or n%3==0):
        return 0  
    i=5
    while(i*i<=n):
        if (n%i==0 or n%(i+2)==0):
            return 0
        i=i+6  
    return 1
	
n=input()
print(str(int(n)%sum(int(x) for x in n)==0).lower())

def isPrime(n):
    return n>1 and all(n%i for i in itertools.islice(itertools.count(2),int(math.sqrt(n)-1)))