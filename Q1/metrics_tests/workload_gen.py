import random
import string

NUMBER_OF_THREADS = 100
TREE_SIZE = 10
ENTRIES_IN_FILE = 100
def get_random_string():
    length = random.randint(1,TREE_SIZE)
    letters = string.ascii_lowercase
    result_str = ''.join(random.choice(letters) for i in range(length))
    # print("Random string of length", length, "is:", result_str)
    return(result_str)


for i in range(NUMBER_OF_THREADS):
    number_of_entries = random.randint(1,ENTRIES_IN_FILE)
    file_read = open("./workload/find/"+str(i+1)+".txt", "w")
    file_initial = open("./workload/initial/"+str(i+1)+".txt", "w")
    file_pref = open("./workload/pref/"+str(i+1)+".txt", "w")
    file_rem = open("./workload/rem/"+str(i+1)+".txt", "w")
    for j in range(number_of_entries):
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



