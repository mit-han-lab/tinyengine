cd ../../
PYTHONPATH=$PYTHONPATH:${pwd} python examples/detection_fpn.py
cp -r TinyEngine examples/openmv_person_detection/openmv/src/omv/modules/
mv codegen/ examples/openmv_person_detection/openmv/src/omv/modules/TinyEngine/
cd examples/openmv_person_detection
