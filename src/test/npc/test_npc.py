import parametrize_from_file as pff
from csv import DictReader
from collections import defaultdict
import subprocess


def load_csv(path):
    with open(path) as f:
        cases = list(DictReader(f))
    return defaultdict(lambda: cases)


pff.add_loader(".csv", load_csv)


def prefix_np(filename):
    return f"src/test/npc/accepted-programs/{filename}.np"


def prefix_asm(filename):
    return f"src/test/npc/accepted-programs/{filename}.s"


def exec_filename(filename):
    return f"src/test/npc/accepted-programs/{filename}"


def compile_program(filename):
    subprocess.run(["./npc", filename, ">", "/tmp/out"], stdout=subprocess.DEVNULL)


def run_program(asm_filename, exec_filename):
    subprocess.run(["gcc", asm_filename, "lib.c", "-o", exec_filename])
    subprocess.call([exec_filename])


@pff.parametrize
def test_program(capfd, program, expected_output):
    compile_program(prefix_np(program))
    run_program(prefix_asm(program), exec_filename(program))
    out, err = capfd.readouterr()
    assert out == expected_output
