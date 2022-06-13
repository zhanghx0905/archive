'''Global config parameters'''
gamma = 200  # for smooth
eps = 1e-8  # a small float number to avoid underflow
band_width = 6

# pausing punctuation in chinese
pausing_punc = "！？：。，、；—…《》（）“”"
# model path
char_table_path = "..\model\char-table.pkl"

# for polyphone model
pinyin2char_path = "..\model\pinyin2char.pkl"
char2pinyin_path = "..\model\char2pinyin.pkl"
