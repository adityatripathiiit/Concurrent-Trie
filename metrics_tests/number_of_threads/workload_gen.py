import random
import string

TREE_SIZE = 6
ENTRIES_IN_FILE = 500000

def get_random_string():
    length = random.randint(1,TREE_SIZE)
    # put your letters in the following string
    sample_letters = 'abcdefghi'
    result_str = ''.join((random.choice(sample_letters) for i in range(length)))
    return(result_str)


i=0
file_read = open("./workload/find/"+str(i+1)+".txt", "w")
file_initial = open("./workload/initial/"+str(i+1)+".txt", "w")
file_pref = open("./workload/pref/"+str(i+1)+".txt", "w")
file_rem = open("./workload/rem/"+str(i+1)+".txt", "w")
for j in range(ENTRIES_IN_FILE):
    file_read.write(get_random_string()+ "\n")
    file_initial.write(get_random_string()+ "\n")
    file_initial.write(str(random.randint(1,1000))+ "\n")
    file_pref.write(get_random_string()+ "\n")
    file_rem.write(get_random_string()+ "\n")
file_read.write("--"+ "\n")
file_read.close()
file_initial.write("--"+ "\n")
file_initial.close()
file_pref.write("--"+ "\n")
file_pref.close()
file_rem.write("--"+ "\n")
file_rem.close()



