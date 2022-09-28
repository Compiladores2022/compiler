
def fetch_output(filename):
    filename = filename[:-3]
    filename = filename.split("-", 1)[1]
    return filename


# def capture_output():

if __name__ == "__main__":
    res = fetch_output("test_a-4.np")
    print(res)
