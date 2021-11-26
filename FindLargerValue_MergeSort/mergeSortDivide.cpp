/*
	Divide and Conquer merge sort to count when larger values come before a smaller one in a list
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

int merge(vector<int>& arr, vector<int>& temp, int left, int mid,
	int right);
int mergeSort(vector<int>& arr, vector<int>& temp, int left, int right);

int main()
{
	//get input filename, read input file with number of elements and elements, put in int array
	ifstream in;
	string inputFile = "input.txt";
	in.open(inputFile);
	string s;
	vector<int> arr;
	int size = 0;
	int i = 0;
	if (in)
	{
		cout << "File Opened Successfully" << endl;

		cout << "Original File" << endl;
		while (getline(in, s))
		{
			cout << s << ' ';
			if (i > 0)
				arr.push_back(stoi(s));
			else
				size = stoi(s);
			i++;
		}

	}

	i = 0;
	cout << endl << "Input array elements" << endl;
	while (i < arr.size())
	{
		cout << arr[i] << ' ';
		i++;
	}
	cout << endl;
	vector<int> temp(size);
	int ans = mergeSort(arr, temp, 0, int(arr.size() - 1));
	cout << "Count is: " << ans << endl;

	return 0;
}

/*
	Time complexity for mergeSort divide and conquer count for sorts to get
	count desired for elements that are smaller after a larger one in an
	array is O(n log(n) ) because the list is traversed once and it is being divided
	in half
*/

int mergeSort(vector<int>& arr, vector<int>& temp, int left, int right)
{
	int mid, count = 0;
	if (right > left) {

		//midpoint of array to make 2 arrays
		mid = (right + left) / 2;

		/* 
			count number of times array needs to be sorted
			for left and right array
		*/
		count += mergeSort(arr, temp, left, mid);
		count += mergeSort(arr, temp, mid + 1, right);

		//put two arrays together to get a total count
		count += merge(arr, temp, left, mid + 1, right);
	}
	return count;
}

int merge(vector<int>& arr, vector<int>& temp, int left, int mid,
	int right)
{
	int i, j, k;
	int count = 0;
	i = left;
	j = mid;
	k = left;

	//check both halves of array to check where needs
	//to be sorted
	while ((i <= mid - 1) && (j <= right)) {
		if (arr[i] <= arr[j]) {
			temp[k++] = arr[i++];
		}
		else {
			temp[k++] = arr[j++];

			/* 
				if element in right of array is greater,
				counts as a sort
			*/
			count = count + (mid - i);
		}
	}

	//put any elements that did not require sorting back into array
	while (i <= mid - 1)
		temp[k++] = arr[i++];

	while (j <= right)
		temp[k++] = arr[j++];

	for (i = left; i <= right; i++)
		arr[i] = temp[i];

	return count;
}
