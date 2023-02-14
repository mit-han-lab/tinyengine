import os
from typing import List

import setuptools


def _get_requirements_from_file(fname):
    reqs = []
    if os.path.exists(fname):
        with open(fname, "r") as f:
            f_text = f.read()
            reqs.append(f_text.strip().split("\n"))

    return reqs


def _get_install_requires() -> List[str]:
    requirement_package = []
    requirements_files = ["requirements.txt"]
    for req_file in requirements_files:
        for reqs in _get_requirements_from_file(req_file):
            requirement_package += reqs

    return requirement_package


setuptools.setup(
    name="tinyengine",
    version="0.0.1",
    install_requires=_get_install_requires(),
    python_requires=">=3.6",
)
