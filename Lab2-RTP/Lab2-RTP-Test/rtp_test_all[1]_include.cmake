if(EXISTS "/home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/rtp_test_all[1]_tests.cmake")
  include("/home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/rtp_test_all[1]_tests.cmake")
else()
  add_test(rtp_test_all_NOT_BUILT rtp_test_all_NOT_BUILT)
endif()
