package main

import (
	"fmt"
	"os"
	"strconv"
)

// fibonacci function that returns a function that returns an int.
func fibonacci() func() int {
	var n []int
	return func() int {
		l_n := len(n)
		switch v := l_n; v {
		case 0:
			n = append(n, 0)
		case 1:
			n = append(n, 1)
		default:
			n = append(n, n[l_n-2]+n[l_n-1])
		}
		//fmt.Printf("added %d\n", n[l_n])
		return n[l_n]
	}
}

func min(x int64, y int64) int64 {
	if x <= y {
		return x
	} else {
		return y
	}
}

func max(x int64, y int64) int64 {
	if x > y {
		return x
	} else {
		return y
	}
}

func sum(numbers []int64) int64 {
	len_ := int64(len(numbers))
	sum_ := int64(0)
	for l := int64(0); l < len_; l++ {
		sum_ += numbers[l]
	}
	return sum_
}

func main() {
	err := (error)(nil)
	if len(os.Args) < 2 {
		panic(fmt.Sprintf("[error] missing option arg"))
	}
	option := os.Args[1]
	switch option {
	case "fibonacci":
		iterations := 10
		if len(os.Args) >= 3 {
			if iterations, err = strconv.Atoi(os.Args[2]); err != nil {
				panic(fmt.Sprintf("[error] invalid 'iterations' arg"))
			}
		}
		f := fibonacci()
		for i := 0; i < iterations; i++ {
			fmt.Println(f())
		}
	case "min", "max":
		if len(os.Args) < 4 {
			panic(fmt.Sprintf("[error] missing x / y arguments"))
		}
		var f func(int64, int64) int64
		if option == "max" {
			f = max
		} else {
			f = min
		}
		var x, y int64
		if x, err = strconv.ParseInt(os.Args[2], 10, 64); err != nil {
			panic(fmt.Sprintf("[error] invalid numberic 'x' arg '%v'", os.Args[2]))
		}
		if y, err = strconv.ParseInt(os.Args[3], 10, 64); err != nil {
			panic(fmt.Sprintf("[error] invalid numberic 'y' arg '%v'", os.Args[3]))
		}
		fmt.Println(f(x, y))
	case "sum":
		if len(os.Args) < 2 {
			panic(fmt.Sprintf("[error] missing numberic argument(s) for summation"))
		}
		numbers := make([]int64, 0)
		for _, s := range os.Args[2:] {
			if x, err := strconv.ParseInt(s, 10, 64); err != nil {
				fmt.Printf("[info] skipping invalid numberic '%v'", s)
			} else {
				numbers = append(numbers, x)
			}
		}
		fmt.Println(sum(numbers))
	default:
		fmt.Printf("[error] unsupported option '%v'\n", option)
	}
}
