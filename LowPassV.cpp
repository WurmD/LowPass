
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

vector<int> boxesForGauss(float sigma, int n)  // standard deviation, number of boxes
{
	auto wIdeal = sqrt((12 * sigma*sigma / n) + 1);  // Ideal averaging filter width 
	int wl = floor(wIdeal);
	if (wl % 2 == 0)
		wl--;
	int wu = wl + 2;

	auto mIdeal = (12 * sigma*sigma - n * wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	int m = round(mIdeal);
	// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	vector<int> sizes(n);
	for (auto i = 0; i < n; i++)
		sizes.emplace_back(i < m ? wl : wu);
	return sizes;
}

void boxBlurH_4(vector<unsigned char>& scl, vector<unsigned char>& tcl, int w, int h, int r) {
	float iarr = 1.f / (r + r + 1);
	for (auto i = 0; i < h; i++) {
		auto ti = i * w, li = ti, ri = ti + r;
		auto fv = scl[ti], lv = scl[ti + w - 1];
		auto val = (r + 1)*fv;
		for (auto j = 0; j < r; j++) val += scl[ti + j];
		for (auto j = 0; j <= r; j++) { val += scl[ri++] - fv;   tcl[ti++] = round(val*iarr); }
		for (auto j = r + 1; j < w - r; j++) { val += scl[ri++] - scl[li++];   tcl[ti++] = round(val*iarr); }
		for (auto j = w - r; j < w; j++) { val += lv - scl[li++];   tcl[ti++] = round(val*iarr); }
	}
}
void boxBlurT_4(vector<unsigned char>& scl, vector<unsigned char>& tcl, int w, int h, int r) {
	float iarr = 1.f / (r + r + 1);
	for (auto i = 0; i < w; i++) {
		auto ti = i, li = ti, ri = ti + r * w;
		auto fv = scl[ti], lv = scl[ti + w * (h - 1)];
		auto val = (r + 1)*fv;
		for (auto j = 0; j < r; j++) val += scl[ti + j * w];
		for (auto j = 0; j <= r; j++) { val += scl[ri] - fv;  tcl[ti] = round(val*iarr);  ri += w; ti += w; }
		for (auto j = r + 1; j < h - r; j++) { val += scl[ri] - scl[li];  tcl[ti] = round(val*iarr);  li += w; ri += w; ti += w; }
		for (auto j = h - r; j < h; j++) { val += lv - scl[li];  tcl[ti] = round(val*iarr);  li += w; ti += w; }
	}
}
void boxBlur_4(vector<unsigned char>& scl, vector<unsigned char>& tcl, int w, int h, int r) {
	for (auto i = 0; i < scl.size(); i++) tcl[i] = scl[i];
	boxBlurH_4(tcl, scl, w, h, r);
	boxBlurT_4(scl, tcl, w, h, r);
}
void gaussBlur_4(vector<unsigned char>& scl, vector<unsigned char>& tcl, int w, int h, int r) {
	auto bxs = boxesForGauss(r, 3);
	boxBlur_4(scl, tcl, w, h, (bxs[0] - 1) / 2);
	boxBlur_4(tcl, scl, w, h, (bxs[1] - 1) / 2);
	boxBlur_4(scl, tcl, w, h, (bxs[2] - 1) / 2);
}

vector<unsigned char> LowPass(vector<unsigned char>& source, int w, int h, int radius)
{
	vector<unsigned char> lowpass = source; // copy constructor
	vector<unsigned char> target(source.size(), 0);
	gaussBlur_4(lowpass, target, w, h, radius);

	return target;
}

int main()
{

	int w = 30, h = 5;
	vector<unsigned char> v(w*h, 0);
	v[2 * w + 2] = 255;

	auto vLow = LowPass(v, w, h, 2);

	cout << "Original: ";
	for (auto i = 2*w; i < 2*w+30; i++)
		cout << int(v[i]) << " ";
	cout << endl;

	cout << "LowPass : ";
	for (auto i = 2 * w; i < 2 * w + 30; i++)
		cout << int(vLow[i]) << " ";
	cout << endl;

	return 0;
}
