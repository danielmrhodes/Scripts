for i in {1..14145..1}
do
    echo "step $i" 
    root -l -q WriteME3D.C > /dev/null 2>&1
    gosia < kr80.INTI.inp > /dev/null 2>&1
    gosia < kr80.MAP.inp > /dev/null 2>&1
    gosia < kr80_1.inp > /dev/null 2>&1
    gosia < kr80.INTI.inp > /dev/null 2>&1
    gosia < kr80.MAP.inp > /dev/null 2>&1
    gosia < kr80_2.inp > /dev/null 2>&1
    root -l -q WriteOutput.C > /dev/null 2>&1
done
