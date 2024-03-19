filename=s21_grep.c;
filenames=s21_grep.c s21_grep_lib.h
filename_for_f=test_ptrn_grep.txt
nofile=no_file.c 
bool=false;

echo ------------------ 1 grep -e int "$filename"
diff  <(grep -e int "$filename") <(./s21_grep -e int "$filename") -s

echo ------------------ 2 grep -e int -e main "$filename"
diff  <(grep -e int -e main "$filename") <(./s21_grep -e int -e main "$filename") -s 

echo ------------------ 3 grep -i INT "$filename"
diff  <(grep -i INT "$filename") <(./s21_grep -i INT "$filename") -s 

echo ------------------ 4 grep -v int "$filename"
diff  <(grep -v int "$filename") <(./s21_grep -v int "$filename") -s

echo ------------------ 5 grep -c int "$filename"
diff  <(grep -c int "$filename") <(./s21_grep -c int "$filename") -s

echo ------------------ 6 grep -l int "$filename"
diff  <(grep -l int "$filename") <(./s21_grep -l int "$filename") -s

echo ------------------ 7 grep -n int "$filename"
diff  <(grep -n int "$filename") <(./s21_grep -n int "$filename") -s 

echo ------------------ 8 grep -h int "$filename"
diff  <(grep -h int "$filename") <(./s21_grep -h int "$filename") -s

echo ------------------ 9 grep -s int "$nofile"
diff  <(grep -s int "$nofile") <(./s21_grep -s int "$nofile") -s

echo ------------------ 10 grep -e regex -e ^print "$filename" -h -f "$filename_for_f"
diff  <(grep -e regex -e ^print "$filename" -h -f "$filename_for_f") <(./s21_grep -e regex -e ^print "$filename" -h -f "$filename_for_f") -s

echo ------------------ 11 grep -o int "$filename"
diff  <(grep -o int "$filename") <(./s21_grep -o int "$filename") -s
