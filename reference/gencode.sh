rm -rf codegen
cd ..
PYTHONPATH=${PYTHONPATH}:$(pwd) python examples/vww.py
mv codegen reference/lib/TinyEngine
cd reference
