import pytest
import os

@pytest.fixture(scope='session')
def file():
    filename = "/tmp/testfile.txt"
    with open(filename, "w") as f:
        f.write("this is my file")
    try:
        yield filename
    finally:
        os.remove(filename)

@pytest.fixture(scope='session')
def resource(file):
    with open(file, "r") as f:
        yield f

def test_foo(resource):
    assert "my file" in resource.readline()

def test_foo2(resource):
    assert "my file" in resource.readline()
