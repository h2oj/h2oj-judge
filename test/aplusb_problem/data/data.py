from cyaron import *

def gen(file_prefix, data_id, func, opt = {}):
    data = IO(file_prefix=file_prefix, data_id=data_id)
    func(data, opt)
    print('Generated [' + file_prefix + str(data_id) + ']')

def gen0(data, opt):
    a, b = randint(1, 10**10), randint(1, 10**10)
    data.input_writeln(a, b)
    data.output_writeln(a + b)

if __name__ == '__main__':
    #gen('aplusb', 101, gen0)
    #gen('aplusb', 102, gen0)
    
    gen('aplusb', 1, gen0)
    gen('aplusb', 2, gen0)
    gen('aplusb', 3, gen0)
    gen('aplusb', 4, gen0)
    gen('aplusb', 5, gen0)
    gen('aplusb', 6, gen0)
    gen('aplusb', 7, gen0)
    gen('aplusb', 8, gen0)
    gen('aplusb', 9, gen0)
    gen('aplusb', 10, gen0)


