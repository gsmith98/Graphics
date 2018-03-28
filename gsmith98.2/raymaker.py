import math


def main():
    filename = raw_input("filename: ")
    with open(filename) as f:
        content = f.readlines()

        funcdict = {"rotx": rotx,
                    "roty": roty,
                    "rotz": rotz,
                    "translate": translate,
                    "scale": scale}

        beginning = "#group_begin\n"
        end = "#ray_file_instance 1\n#group_end\n"
        lastmat = ident()
        for line in content:
            things = line.split()
            thismat = funcdict[things[0]](things[1])
            lastmat = mult(thismat, lastmat)

        with open("outray", "w") as out:
            out.write(beginning + strmatrix(lastmat) + end)

def ident():
    return [[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]]

def mult(mat1, mat2):
    result = [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
    for i in range(4):
        for j in range(4):
            for k in range(4):
                result[i][j] += mat1[i][k]*mat2[k][j]
    return result

def translate(xyz):
    vals = map(float, xyz.split(","))
    matrix = ident()
    matrix[0][3] = vals[0]
    matrix[1][3] = vals[1]
    matrix[2][3] = vals[2]
    return ret(matrix)

def scale(xyz):
    vals = map(float, xyz.split(","))
    matrix = ident()
    matrix[0][0] = vals[0]
    matrix[1][1] = vals[1]
    matrix[2][2] = vals[2]
    return ret(matrix)

def ret(matrix):
    strmat = ""
    for row in matrix:
        for entry in row:
            if abs(entry) < .000001:
                entry = 0
            strmat += str(entry) + "\t"
        strmat += "\n"
    return matrix

def strmatrix(matrix):
    strmat = ""
    for row in matrix:
        for entry in row:
            if abs(entry) < .000001:
                entry = 0
            strmat += str(entry) + "\t"
        strmat += "\n"
    return strmat

def rotz(deg):
    cos = math.cos(math.radians(float(deg)))
    sin = math.sin(math.radians(float(deg)))
    matrix = ident()
    matrix[0][0] = cos
    matrix[0][1] = -sin
    matrix[1][0] = sin
    matrix[1][1] = cos
    return ret(matrix)

def roty(deg):
    cos = math.cos(math.radians(float(deg)))
    sin = math.sin(math.radians(float(deg)))
    matrix = ident()
    matrix[0][0] = cos
    matrix[0][2] = sin
    matrix[2][0] = -sin
    matrix[2][2] = cos
    return ret(matrix)

def rotx(deg):
    cos = math.cos(math.radians(float(deg)))
    sin = math.sin(math.radians(float(deg)))
    matrix = ident()
    matrix[1][1] = cos
    matrix[1][2] = -sin
    matrix[2][1] = sin
    matrix[2][2] = cos
    return ret(matrix)

if __name__ == "__main__":
    main()