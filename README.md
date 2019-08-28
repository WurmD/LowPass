# LowPass
Fastest Gaussian Blur implementation in C++, but currently not very fast, there is too much delay at `		for (auto j = r + 1; j < h - r; j++) { val += scl[ri] - scl[li];  tcl[ti] = round(val*iarr);  li += w; ri += w; ti += w; }`


Inspired by http://blog.ivank.net/fastest-gaussian-blur.html 
