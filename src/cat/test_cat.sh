filename=test_case_cat.txt;


echo ------------------ 1 cat -b  "$filename"
diff  <(cat -b "$filename") <(./s21_cat -b "$filename") -s

echo ------------------ 2 cat -e "$filename"
diff  <(cat -e "$filename") <(./s21_cat -e "$filename") -s 

echo ------------------ 3 cat -n  "$filename"
diff  <(cat -n  "$filename") <(./s21_cat -n  "$filename") -s

echo ------------------ 4 cat -s  "$filename"
diff  <(cat -s  "$filename") <(./s21_cat -s  "$filename") -s

echo ------------------ 5 cat -t  "$filename"
diff  <(cat -t  "$filename") <(./s21_cat -t  "$filename") -s

