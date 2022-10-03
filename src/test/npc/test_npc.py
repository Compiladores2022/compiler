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


def compile_program_no_output(filename):
    status = subprocess.run(["./npc", filename], stdout=subprocess.DEVNULL)
    return status.returncode


def compile_program_with_output(filename):
    status = subprocess.run(["./npc", filename])
    return status.returncode


def run_program(asm_filename, exec_filename):
    subprocess.run(["gcc", asm_filename, "lib.c", "-o", exec_filename])
    subprocess.call([exec_filename])


@pff.parametrize("test_npc.csv")
def test_compile_success(program, expected_output):
    exit_status = compile_program_no_output(prefix_np(program, "accepted"))
    assert exit_status == 0


@pff.parametrize("test_npc_neg.csv")
def test_compile_failure(program, expected_output):
    exit_status = compile_program_no_output(prefix_np(program, "rejected"))
    assert exit_status == 1


@pff.parametrize("test_npc.csv")
def test_program_output(capfd, program, expected_output):
    compile_program_no_output(prefix_np(program, "accepted"))
    run_program(prefix_asm(program), exec_filename(program))
    out, err = capfd.readouterr()
    assert out == expected_output


@pff.parametrize("test_npc_neg.csv")
def test_error_message(capfd, program, expected_output):
    compile_program_with_output(prefix_np(program, "rejected"))
    out, err = capfd.readouterr()
    assert expected_output in out
