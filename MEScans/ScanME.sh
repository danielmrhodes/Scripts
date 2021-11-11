for i in {1..21..1}
do
    echo "step $i" 
    root -l -q WriteME.C > /dev/null 2>&1
    gosia < ge80.INTI.inp > /dev/null 2>&1
    gosia < ge80.MAP.inp > /dev/null 2>&1
    gosia < ge80_1.inp > /dev/null 2>&1
    gosia < ge80.INTI.inp > /dev/null 2>&1
    gosia < ge80.MAP.inp > /dev/null 2>&1
    gosia < ge80_2.inp > /dev/null 2>&1
    gosia < ge80.INTI.inp > /dev/null 2>&1
    gosia < ge80.MAP.inp > /dev/null 2>&1
    gosia < ge80_3.inp > /dev/null 2>&1
    gosia < ge80.INTI.inp > /dev/null 2>&1
    gosia < ge80.MAP.inp > /dev/null 2>&1
    gosia < ge80_4.inp > /dev/null 2>&1
    gosia < ge80.INTI.inp > /dev/null 2>&1
    gosia < ge80.MAP.inp > /dev/null 2>&1
    gosia < ge80_2.inp > /dev/null 2>&1
    gosia < ge80.INTI.inp > /dev/null 2>&1
    gosia < ge80.MAP.inp > /dev/null 2>&1
    gosia < ge80_3.inp > /dev/null 2>&1
    root -l -q WriteOutput.C > /dev/null 2>&1
done
