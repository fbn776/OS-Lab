echo "a - Add";
echo "b - Subtract";
echo "c - Multiply";
echo "d - Divide";
echo "e - Modulo";

while true ; do
	read -p "Choice: " choice

	read -p "Num 1: " num1
	read -p "Num 2: " num2

	case $choice in
		"a")
			echo "Result $((num1 + num2))"
		;;
		"b")
			echo "Result $((num1 - num2))"
		;;
		"c")
			echo "Result $((num1 * num2))";;
		"d")
			if [ "$num2" -eq 0 ] ; then
				echo "Division by zero!!"
			else
				echo "Result $((num1 / num2))"
			fi
		;;
		"e")
			echo "Result $((num1 % num2))"
		;;
		*)
			echo "Invalid choice"
			break
		;;
	esac
done

