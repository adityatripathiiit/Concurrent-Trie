# Results For the Q1: Concurrent Trie

## Performace Metrics: 

1) ### Number of concurrent threads
    Read Intensive             |  Read Intensive cumulative averaged
    :-------------------------:|:-------------------------:
    ![](metrics_tests/number_of_threads/plots/nthreads_comp_read.png)  |  ![](metrics_tests/number_of_threads/plots/nthreads_comp_avg_read.png)

    
    Write Intensive             |  Write Intensive cumulative averaged
    :-------------------------:|:-------------------------:
    ![](metrics_tests/number_of_threads/plots/nthreads_comp_write.png)  |  ![](metrics_tests/number_of_threads/plots/nthreads_comp_avg_write.png)

    Mixed Read & Write workload|  Mixed Read & Write workload cumulative averaged
    :-------------------------:|:-------------------------:
    ![](metrics_tests/number_of_threads/plots/nthreads_comp_50-50.png)  |  ![](metrics_tests/number_of_threads/plots/nthreads_comp_avg_50-50.png)


2) ### Size of the trie

    Read Intensive             |  Read Intensive cumulative averaged
    :-------------------------:|:-------------------------:
    ![](metrics_tests/trie_size/plots/size_comp_read.png)  |  ![](metrics_tests/trie_size/plots/size_comp_Avg_read.png)

    
    Write Intensive             |  Write Intensive cumulative averaged
    :-------------------------:|:-------------------------:
    ![](metrics_tests/trie_size/plots/size_comp_write.png)  |  ![](metrics_tests/trie_size/plots/size_comp_Avg_write.png)

    Mixed Read & Write workload|  Mixed Read & Write workload cumulative averaged
    :-------------------------:|:-------------------------:
    ![](metrics_tests/trie_size/plots/size_comp_50-50.png)  |  ![](metrics_tests/trie_size/plots/size_comp_Avg_50-50.png)