// Kevin Sittser
// 525003900
// CSCE420
// Due: February 14, 2019
// README.txt


Programs compile and run on TAMU CSE compute server with compile command "g++-8.2.0 -std=c++2a hw2pr1.cpp -o filename" (or hw2pr2 or hw2pr3) and run command "./filename"

Due to initial misunderstanding, trees calculate and store value for each node, including non-leaves; however, actual problem solutions don't ever actually use these values-- only the leaf values are actually used

Problem 3 is currently set to a search depth of 9 moves.  Any more causes the program to take too long to make a move

As is, Brady's starting placement may take around 20 seconds to occur; moves after this are much faster