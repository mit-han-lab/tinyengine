PYTHONPATH=${PYTHONPATH}:$(pwd) python examples/tiny_training.py -f assets/49kb-int8-graph.json -D assets/full-int8-params.pkl -QAS assets/scale.json -m -g -d -FR
