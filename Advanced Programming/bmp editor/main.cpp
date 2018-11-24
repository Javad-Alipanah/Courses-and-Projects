#include<iostream>
#include<cstring>
#include<vector>

using namespace std;

void make_pic_bw(unsigned int size, vector<unsigned char>& pic);
void mirror_pic(unsigned int size, vector<unsigned char>& pic);
void rotate_pic(vector<unsigned char>& pic);
unsigned int find_size_of_picture(vector<unsigned char>& pic);
unsigned int find_pix_offset(vector<unsigned char> pic);
unsigned int find_height(vector<unsigned char>& pic);
unsigned int find_width(vector<unsigned char>& pic);
void reverse(unsigned int i, unsigned int j, vector<unsigned char>& pic);
void construct_temp(unsigned int m, unsigned int n, vector<unsigned char> pic, vector< vector<unsigned char> >& temp, unsigned int& idx);
void construct_pic(vector<unsigned char>& pic, vector< vector<unsigned char> > temp);

int main(int argc , char* argv[]) {
	vector<unsigned char> pic;
	unsigned int size = find_size_of_picture(pic);
	for(unsigned int i = pic.size() - 1; i < size; i++)
		pic.push_back(cin.get());
	for(unsigned int j = 0; j < argc - 1; j++ ) {
		if( !strcmp( argv[j + 1] , "1") )
			make_pic_bw(size, pic);
		else if( !strcmp( argv[j + 1] , "2") )
			mirror_pic(size, pic);
		else if( !strcmp( argv[j + 1] , "3") )
			rotate_pic(pic);
	}
	for(unsigned int s = 0; s < size; s++)
		cout << pic[s];	
	return 0;
}

unsigned int find_size_of_picture(vector<unsigned char>& pic) {
	unsigned int size = 0;	
	for(int i = 0; i < 6; i++) {
		if(i < 2) {
			pic.push_back(cin.get());
			continue;
		}
		pic.push_back(cin.get());
		size += (unsigned char)pic[i] << (i - 2) * 8;
	}
	return size;
}

unsigned int find_width(vector<unsigned char>& pic) {
	unsigned int width = 0;
	for(unsigned int i = 18; i <= 21; i++)
		width += (unsigned char)pic[i] << (i - 18) * 8;
	return width;
}

unsigned int find_height(vector<unsigned char>& pic) {
	unsigned int height = 0;
	for(int i = 22; i <= 25; i++)
		height += (unsigned char)pic[i] << (i - 22) * 8;
	return height;
}

unsigned int find_pix_offset(vector<unsigned char> pic) {
	unsigned int offset = 0;	
	for(unsigned int counter = 10; counter <= 13; counter++)
		offset += (unsigned int)pic[counter] << (counter - 10) * 8;
	return offset;
}

void make_pic_bw(unsigned int size, vector<unsigned char>& pic) {
	unsigned int offset = find_pix_offset(pic), width = find_width(pic);
	unsigned int weight = 0, flag = 0;	
	for(unsigned int i = offset; i < size; i += width * 3) {
		while((unsigned int)pic[i] == 0) {
			if( i == size ) break;
			i++;
		}
		for(unsigned int j = i; j < i + width * 3; j += 3) {
			weight = ((unsigned char)pic[j] + (unsigned char)pic[j + 1] + (unsigned char)pic[j + 2]) / 3;
			pic[j] = pic[j + 1] = pic[j + 2] = weight;
		}
	}
}

void reverse(unsigned int i, unsigned int j, vector<unsigned char>& pic) {
	unsigned char tmp;	
	for(i = i; i < j; i += 3) {
		for(int k = 2; k >= 0; k--) {
			tmp = pic[i + (2 - k)];
			pic[i + (2 - k)] = pic[j - k];
			pic[j - k] = tmp;
		}
		j -= 3;
	}
}


void mirror_pic(unsigned int size, vector<unsigned char>& pic) {
	unsigned int offset = find_pix_offset(pic), width = find_width(pic);
	for(unsigned int i = offset; i < size; i += width * 3) {
		while((unsigned int)pic[i] == 0) {
			if( i == size ) break;
			i++;
		}
		reverse(i, i + width * 3 - 1, pic);
	}
}

void rotate_pic(vector<unsigned char>& pic) {
	vector< vector<unsigned char> > temp;
	vector<unsigned char>tmp;
	unsigned int width = find_width(pic), offset = find_pix_offset(pic), size = pic.size(), idx = 0, height = find_height(pic), friction = 0;
	for(unsigned int i = offset; i < size; i += width * 3) {
		for(int j = 0; j < 4 - (width * 3) % 4; j++) {
			if( i == size - 1 || i == offset || (width * 3) % 4 == 0) break;
			friction++;
		}
		temp.push_back(tmp);
		construct_temp(i + friction , i + friction + width * 3 - 1 , pic, temp, idx);
	}
	construct_pic(pic, temp);	
}


void construct_temp(unsigned int m, unsigned int n, vector<unsigned char> pic, vector< vector<unsigned char> >& temp, unsigned int& idx) {	
	for(unsigned int i = 0; i <= n - m; i++)
		temp[idx].push_back(pic[m + i]);
	idx++;
}

void construct_pic(vector<unsigned char>& pic, vector< vector<unsigned char> > temp) {
	vector< vector<unsigned char> > rotated;	
	vector<unsigned char> tmp;
	unsigned char tmp2;
	unsigned int width = find_width(pic), height = find_height(pic), offset = find_pix_offset(pic), friction = 0, size = pic.size(), flag = 0, i , j;
	for(int i = 0; i < width; i++) {
		rotated.push_back(tmp);
		for(int j = 0; j < height; j++) {
			rotated[i].push_back(temp[j][width * 3 - 3 * i - 3]);
			rotated[i].push_back(temp[j][width * 3 - 3 * i - 2]);
			rotated[i].push_back(temp[j][width * 3 - 3 * i - 1]);
		}
	}
	for(i = 0; i < width; i++) {
		for(j = 0; j < height * 3; j++)
			if(!flag)
				pic[offset + i * height * 3 + j + friction] = rotated[i][j];
			else
				pic.push_back(rotated[i][j]);
		for(int m = 0; m < 4 - (height * 3) % 4; m++) {
			if((4 - (height * 3) % 4) == 4)
				break;
			if(size - 1 >= offset + i * height * 3 + j + friction) {
				if(size - 1 == offset + i * height * 3 + j + friction) flag = 1;
				pic[offset + i * height * 3 + j + friction] = (unsigned char)0;
				friction++;
			}
			else {
				flag = 1;
				pic.push_back((unsigned char)0);
				friction++;
			}
		}
	}
	for(int i = 18; i <= 21; i++) {
		tmp2 = pic[i];
		pic[i] = pic[i + 4];
		pic[i + 4] = tmp2;
	}
	for(int i = 2; i < 6; i++)
		pic[i] = (unsigned char) (((pic.size() - 1) >> 8 * (i - 2)) % 256);
}

