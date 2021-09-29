import os

extList = [
    "obj",
    "exe",
    "pdb",
    "ilk",
    "exp",
    "log",
]

if __name__ == "__main__":
    for root, dirs, files in os.walk(".", topdown=False):
        for name in files:
            filePath = os.path.join(root, name)
            _, ext = os.path.splitext(filePath)
            if ext == "":
                ext = "<empty_ext>"
            else:
                ext = ext[1:]
            if ext in extList:
                print("delete file {}".format(filePath))
                os.remove(filePath)