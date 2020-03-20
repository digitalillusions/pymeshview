import pymeshview
import meshio
import tkinter as tk
from tkinter.filedialog import askopenfilename


def main():
    window = pymeshview.Window()
    tk.Tk().withdraw()
    meshfile = askopenfilename()
    mesh = meshio.read(meshfile)
    data = pymeshview.TetMeshBuffer()
    data.bufferCells(mesh.points, mesh.cells[0].data)
    window.setData(data)
    window.run()


if __name__ == '__main__':
    main()
