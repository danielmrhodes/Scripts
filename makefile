.PHONY: all

RUN_NUMBERS = 59 62 63 64 65 67 68 69 70 72 73 74 75 77 78 79 80 81 82 83 84 85 86 87 89 91 92 93 94 96 97 98 99 100 101 102 103 104 105 107 108 110 111 112 113 114 115 116 117 118 119 120 121 122 124 125 126 129 130 131 132 133 134 135 136 137 142 143 144 145 146 147 148 149 150 152 153 154 155 156 157 158 159 160 161 165 166 167 168 169 170 171 172 173 174 175 176 177 178 179 180 181 182 184 185 187 188 189 191 192 193 194 195 196 198 199 201 202 203 205 207 208 210 211 212 213 214 215 216 217 218 219 220 221 222 224 226 227 228 229 230 231 232 233 234 236 237 238 239 240 244 245 246 247 248 249 251 252 253 255 256 257 258 259 260 261 262 263 264 265 266 268 269 270 271

HIST_FILES = $(patsubst %,hist%.root,$(RUN_NUMBERS))
RUN_FILES = $(patsubst %,run%.root,$(RUN_NUMBERS))

DATA_DIR = /mnt/analysis/pecan-2015/e19029

GRUT_SYS = /user/rhodesd/GRUTinizer_Janus
HISTOS_FILE = $(GRUT_SYS)/lib/libe19029_OnlineHists.so
FILTER_FILE = $(GRUT_SYS)/lib/libFilter_e18502.so

WORK_DIR = /user/rhodesd/pecan-2015/e19029/80Ge

valfile = $(WORK_DIR)/config/values.val
calfile = $(WORK_DIR)/config/channels.cal
calfileRW = $(WORK_DIR)/config/channels%.cal

cuts = $(WORK_DIR)/cuts/80Ge.cuts \
       $(WORK_DIR)/cuts/80GeUS.cuts \
       $(WORK_DIR)/cuts/196Pt.cuts \
       $(WORK_DIR)/cuts/TE.cuts

#cuts = $(WORK_DIR)/cuts/80Ge.cuts \
       $(WORK_DIR)/cuts/TE.cuts

all: $(RUN_FILES) $(HIST_FILES) master.root

run%.root: $(DATA_DIR)/run%/run*.evt
	grutinizer -lmtq --time-sort-depth 100000000000000 $^ $(FILTER_FILE) $(calfile) $(valfile) -o $@

hist%.root: run%.root $(calfileRW)
	grutinizer -lqH $^ $(HISTOS_FILE) $(valfile) $(cuts)

master.root: $(HIST_FILES)
	gadd $@ $^
