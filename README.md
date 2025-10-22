to run: qsub file_name.pbs
to compile: module load GCC/14.3.0

to watch: 
    while true; do
        clear
        date
        qstat -u $USER
        sleep 10
    done
