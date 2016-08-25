Playing with emscripten to compile [zxing's cpp branch](https://github.com/zxing/zxing/tree/00f634024ceeee591f54e6984ea7dd666fab22ae/cpp) to javascript. It works but it's not optimized: [demo](https://fta2012.github.io/zxing-emscripten/).

To build, assuming you have emscripten installed:

```
cd cpp
mkdir build
cd build
emconfigure cmake ..
emmake make VERBOSE=1
cp ../test.html .
sudo python -m SimpleHTTPServer 80
```

Then open http://localhost/test.html in your browser.
