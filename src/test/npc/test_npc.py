import parametrize_from_file as pff
from csv import DictReader
from collections import defaultdict
import subprocess


def load_csv(path):
    with open(path) as f:
        cases = list(DictReader(f))
    return defaultdict(lambda: cases)


pff.add_loader(".csv", load_csv)


def prefix_np(filename, program_class):
    return f"src/test/npc/{program_class}-programs/{filename}.np"


def prefix_asm(filename):
    return f"src/test/npc/accepted-programs/{filename}.s"


def exec_filename(filename):
    return f"src/test/npc/accepted-programs/{filename}"


def compile_program(filename):
    status = subprocess.run(["./npc", filename], stdout=subprocess.DEVNULL)
    return status.returncode


def run_program(asm_filename, exec_filename):
    subprocess.run(["gcc", asm_filename, "lib.c", "-o", exec_filename])
    subprocess.call([exec_filename])


@pff.parametrize("test_npc.csv")
def test_program(capfd, program, expected_output):
    compile_program(prefix_np(program, "accepted"))
    run_program(prefix_asm(program), exec_filename(program))
    out, err = capfd.readouterr()
    assert out == expected_output


@pff.parametrize("test_npc_neg.csv")
def test_program_neg(program, expected_output):
    out = compile_program(prefix_np(program, "rejected"))
    assert out == int(expected_output)
