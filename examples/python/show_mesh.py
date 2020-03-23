import meshio
import pymeshview
import tkinter as tk
import os
import ffmpeg
from tkinter.filedialog import askopenfilename
import argparse


def convert_kwargs_to_args(**kwargs):
    args = type("Arguments", (object,), {"input": None, "output": None, "verbose": False, "no_highlight": False})
    for arg in kwargs:
        try:
            getattr(args, arg)
        except AttributeError:
            print(f"Unknown argument: {arg}")
        setattr(args, arg, kwargs[arg])

    return args


def cleanup_temp_dir(directory):
    for item in os.listdir(directory):
        os.remove(os.path.join(directory, item))

    os.rmdir(directory)


def show_mesh(args):
    filename = args.input
    if filename is None:
        tk.Tk().withdraw()
        filename = askopenfilename()

    show_window = args.verbose or args.no_highlight
    window = pymeshview.Window(isVisible=show_window)
    data = meshio.read(filename)
    tet_mesh = pymeshview.TetMeshBuffer()
    tet_mesh.bufferCells(data.points, data.cells[0].data)
    window.setData(tet_mesh)
    if not os.path.isdir("tmp"):
        os.makedirs("tmp")

    if args.no_highlight:
        window.run()
    else:
        window.highlight(prefixPath="tmp")

    if args.output is not None:
        ffmpeg.input("tmp/output-frame-%01d.png").filter('fps', fps=30).output(args.output).run(overwrite_output=True)

    cleanup_temp_dir("tmp")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str, default=None, dest='input', help="Mesh file to load")
    parser.add_argument("-o", "--output", type=str, default=None, dest='output', help="Name of output file")
    parser.add_argument("--verbose", action='store_true', dest='verbose', help="Show the window")
    parser.add_argument("--no-highlight", action='store_true', dest='no_highlight',
                        help="Interactively look at mesh instead of generating highlight video")
    args = parser.parse_args()
    print(args)

    show_mesh(args)


if __name__ == '__main__':
    # main()
    args = convert_kwargs_to_args(output="out2.mp4")
    show_mesh(args)
