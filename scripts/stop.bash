kill `ps -ef | grep test.out | grep -v grep | awk '{print $2}'`


while [`ps -ef | grep test.out | grep -v grep | awk '{print $2}' | wc -l` -ne 0 ]
do
#    echo Waiting for processes to die...
    sleep 1
done


for FILE in $(ls ../../csv/*.csv)
do
    sed '$d' $FILE > TEMPFILE_3r489tu230391refe3_TEMPFILE
    mv TEMPFILE_3r489tu230391refe3_TEMPFILE $FILE
done

rm -f TEMPFILE_3r489tu230391refe3_TEMPFILE
