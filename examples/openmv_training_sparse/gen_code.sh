cd ../../
export PYTHONPATH=${PYTHONPATH}:$(pwd)
cp -r TinyEngine examples/openmv_training_sparse/openmv/src/omv/modules/
cd examples/openmv_training_sparse
mkdir codegen
python ../tiny_training.py -f ../../assets/49kb-int8-graph.json -D ../../assets/full-int8-params.pkl -QAS ../../assets/scale.json -m -g -d -FR
mv codegen openmv/src/omv/modules/TinyEngine/
