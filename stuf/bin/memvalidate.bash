#!/usr/bin/env bash
# Version Mon Aug 23 23:31:17 PDT 2021
# Version Mon Mar  2 20:41:49 PST 2020
# Version Mon Apr  1 19:48:21 PDT 2019
# Version Sun Apr  7 13:03:35 PDT 2019
ulimit -c 0
mkdir -p results

passed=0
failed=0
previously_passed=0
all_test_files=tests/*.gpl

if ! [ -f ./gpl ] ; then
  echo "There is no gpl executable in this directory. memvalidate.bash is giving up."
  exit
fi

if ! [ -d tests ] ; then
  echo "There is not a test subdirectory in this directory. memvalidate.bash is giving up."
  exit
fi


# if -all is specified on the command line, remove the results
# directory so that all the .memok files are deleted and all the
# tests are run
#
# if a single number (42) is specified, tests/t042.gpl is run
#  0's are added to pad 1 --> 001 and 10 --> 010
if [[  $# -eq 1  && $1 == "-all" ]] ; then
  rm -f results/*.memok
elif [[  $# -eq 1 ]] ; then
  printf -v test_number "%03d" $1
  all_test_files="tests/t$test_number.gpl"
  rm -f results/t$test_number.memok

  if ! [ -f "$all_test_files" ] ; then
    echo "<$all_test_files> does not exist. Giving up."
    exit
  fi
fi

for i in $all_test_files
do
  bn=$(basename $i)
  memokfile="results/${bn%%.gpl}.memok"
  if [[ -f "$memokfile" ]]
  then
    (( previously_passed++ ))
    continue
  fi
  echo -n $i
  whatever=$(valgrind ./gpl $i 2>/tmp/$$ >/dev/null)
  grep "definitely lost: 0 bytes in 0 blocks" /tmp/$$ 2>&1 1>/dev/null
  first=$?
  grep "indirectly lost: 0 bytes in 0 blocks" /tmp/$$ 2>&1 1>/dev/null
  second=$?
  grep "possibly lost: 0 bytes in 0 blocks" /tmp/$$ 2>&1 1>/dev/null
  third=$?
  grep "no leaks are possible" /tmp/$$ 2>&1 1>/dev/null
  fourth=$?
  if [[ $fourth -eq 0 || $first -eq 0 && $second -eq 0 && $third -eq 0 ]]
  then
    (( passed++ ))
    echo " -- Memory checked. Success!"
    touch "$memokfile"
  else
    (( failed++ ))
    echo " -- ################### Memory leak!"
  fi
  rm -f /tmp/$$
done
total=$passed
(( total += failed ))
echo "Passed $passed out of $total memory tests."
echo "Failed $failed out of $total memory tests."

if [ "$previously_passed" -ne "0" ] ; then
  echo "  $previously_passed memory tests were skipped (passed on previous run)"
  echo "    \"\$ memvalidate.bash -all\" to run all memory tests (recommended before you turn in assignment)"
  echo "    \"\$ memvalidate.bash 42\" to run memory test t042.gpl"
fi
