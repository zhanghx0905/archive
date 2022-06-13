INT_MAX = 2**31 - 1
INT_MIN = -2**31
UNA_TO_ASM = {'-': 'neg', '!': 'seqz', '~': 'not'}
BIN_TO_ASM = {'+i': '\tadd t0, t0, t1',
              '+pleft': '\tslli t1, t1, 2\n'
                        '\tadd t0, t0, t1',
              '+pright': '\tslli t0, t0, 2\n'
                         '\tadd t0, t0, t1',
              '-i': '\tsub t0, t0, t1',
              '-pleft': '\tslli t1, t1, 2\n'
                        '\tsub t0, t0, t1',
              '-pp': '\tsub t0, t0, t1\n'
                     '\tsrai t0, t0, 2',
              '*': '\tmul t0, t0, t1',
              '/': '\tdiv t0, t0, t1',
              '%': '\trem t0, t0, t1',
              '||': '\tsnez t0, t0\n'
                    '\tsnez t1, t1\n'
                    '\tor t0, t0, t1',
              '&&': '\tsnez t0, t0\n'
                    '\tsnez t1, t1\n'
                    '\tand t0, t0, t1',
              '==': '\tsub t0, t0, t1\n'
                    '\tseqz t0, t0',
              '!=': '\tsub t0, t0, t1\n'
                    '\tsnez t0, t0',
              '<': '\tslt t0, t0, t1',
              '<=': '\tsgt t0, t0, t1\n'
                    '\txori t0, t0, 1',
              '>': '\tsgt t0, t0, t1',
              '>=': '\tslt t0, t0, t1\n'
                    '\txori t0, t0, 1',
              '[]p': "\tslli t1, t1, 2\n"
                     "\tadd t0, t0, t1",
              '[]a': "\tmul t1, t1, t2\n"
                     "\tadd t0, t0, t1"}
