#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "linux_parser.h"  // Include the header for LinuxParser

TEST(LinuxParserTest, OperatingSystem) {
  // Create a mock /tmp/os-release file for testing
  const std::string kTestFilePath = "/tmp/test_os_release";
  std::ofstream test_file(kTestFilePath);
  test_file << "PRETTY_NAME=\"Ubuntu 24.04.2 LTS\"\n";
  test_file << "NAME=\"Ubuntu\"\n";
  test_file << "VERSION_ID=\"24.04\"\n";
  test_file << "VERSION=\"24.04.2 LTS (Noble Numbat)\"\n";
  test_file << "VERSION_CODENAME=noble\n";
  test_file << "ID=ubuntu\n";
  test_file << "ID_LIKE=debian\n";
  test_file << "HOME_URL=\"https://www.ubuntu.com/\"\n";
  test_file << "SUPPORT_URL=\"https://help.ubuntu.com/\"\n";
  test_file << "BUG_REPORT_URL=\"https://bugs.launchpad.net/ubuntu/\"\n";
  test_file << "PRIVACY_POLICY_URL=\"https://www.ubuntu.com/legal/"
               "terms-and-policies/privacy-policy\"\n";
  test_file << "UBUNTU_CODENAME=noble\n";
  test_file << "LOGO=ubuntu-logo\n";
  test_file.close();

  // Temporarily override the file path in LinuxParser
  const std::string original_os_path = LinuxParser::kOSPath;
  const_cast<std::string &>(LinuxParser::kOSPath) = kTestFilePath;

  // Call the function and check the result
  std::string os_name = LinuxParser::OperatingSystem();
  EXPECT_EQ(os_name, "Ubuntu 24.04.2 LTS");

  // Restore the original file path
  const_cast<std::string &>(LinuxParser::kOSPath) = original_os_path;

  // Clean up the mock file
  std::remove(kTestFilePath.c_str());
}

TEST(LinuxParserTest, MemoryUtilization) {
  // Create a mock /tmp/meminfo file for testing
  const std::string kTestFilePath = "/tmp/test_meminfo";
  std::ofstream test_file(kTestFilePath);
  test_file << "MemTotal:       32483352 kB\n";
  test_file << "MemFree:        15181044 kB\n";
  test_file << "MemAvailable:   21313964 kB\n";
  test_file << "Buffers:          695504 kB\n";
  test_file << "Cached:          5354424 kB\n";
  test_file << "SwapCached:            0 kB\n";
  test_file << "Active:         11658464 kB\n";
  test_file << "Inactive:        3648132 kB\n";
  test_file << "Active(anon):    9662332 kB\n";
  test_file << "Inactive(anon):        0 kB\n";
  test_file << "Active(file):    1996132 kB\n";
  test_file << "Inactive(file):  3648132 kB\n";
  test_file << "Unevictable:       38296 kB\n";
  test_file << "Mlocked:             136 kB\n";
  test_file << "SwapTotal:       8388604 kB\n";
  test_file << "SwapFree:        8388604 kB\n";
  test_file << "Zswap:                 0 kB\n";
  test_file << "Zswapped:              0 kB\n";
  test_file << "Dirty:              3812 kB\n";
  test_file << "Writeback:             0 kB\n";
  test_file << "AnonPages:       9295320 kB\n";
  test_file << "Mapped:          1589480 kB\n";
  test_file << "Shmem:            405664 kB\n";
  test_file << "KReclaimable:     956748 kB\n";
  test_file << "Slab:            1384792 kB\n";
  test_file << "SReclaimable:     956748 kB\n";
  test_file << "SUnreclaim:       428044 kB\n";
  test_file << "KernelStack:       34656 kB\n";
  test_file << "PageTables:       114624 kB\n";
  test_file << "SecPageTables:      4500 kB\n";
  test_file << "NFS_Unstable:          0 kB\n";
  test_file << "Bounce:                0 kB\n";
  test_file << "WritebackTmp:          0 kB\n";
  test_file << "CommitLimit:    24630280 kB\n";
  test_file << "Committed_AS:   39476408 kB\n";
  test_file << "VmallocTotal:   34359738367 kB\n";
  test_file << "VmallocUsed:      142772 kB\n";
  test_file << "VmallocChunk:          0 kB\n";
  test_file << "Percpu:            12816 kB\n";
  test_file << "HardwareCorrupted:     0 kB\n";
  test_file << "AnonHugePages:         0 kB\n";
  test_file << "ShmemHugePages:    34816 kB\n";
  test_file << "ShmemPmdMapped:        0 kB\n";
  test_file << "FileHugePages:         0 kB\n";
  test_file << "FilePmdMapped:         0 kB\n";
  test_file << "Unaccepted:            0 kB\n";
  test_file << "HugePages_Total:       0\n";
  test_file << "HugePages_Free:        0\n";
  test_file << "HugePages_Rsvd:        0\n";
  test_file << "HugePages_Surp:        0\n";
  test_file << "Hugepagesize:       2048 kB\n";
  test_file << "Hugetlb:               0 kB\n";
  test_file << "DirectMap4k:     1259856 kB\n";
  test_file << "DirectMap2M:    18280448 kB\n";
  test_file << "DirectMap1G:    13631488 kB\n";
  test_file.close();

  // Temporarily override the file path in LinuxParser
  const std::string original_meminfo_path = LinuxParser::kMeminfoFilename;
  const_cast<std::string &>(LinuxParser::kMeminfoFilename) = kTestFilePath;

  // Call the function and check the result
  float memory_utilization = LinuxParser::MemoryUtilization();
  EXPECT_GE(memory_utilization, 0.0f);
  EXPECT_LE(memory_utilization, 1.0f);

  // Restore the original file path
  const_cast<std::string &>(LinuxParser::kMeminfoFilename) =
      original_meminfo_path;

  // Clean up the mock file
  std::remove(kTestFilePath.c_str());
}

TEST(LinuxParserTest, UpTime) {
  // Create a mock /tmp/uptime file for testing
  const std::string kTestFilePath = "/tmp/test_uptime";
  std::ofstream test_file(kTestFilePath);
  test_file << "12345.67 54321.89\n";  // Mock uptime and idle time
  test_file.close();

  // Temporarily override the file path in LinuxParser
  const std::string original_uptime_path = LinuxParser::kUptimeFilename;
  const_cast<std::string &>(LinuxParser::kUptimeFilename) = kTestFilePath;

  // Call the function and check the result
  long uptime = LinuxParser::UpTime();
  EXPECT_GE(uptime, 20);

  // Restore the original file path
  const_cast<std::string &>(LinuxParser::kUptimeFilename) =
      original_uptime_path;

  // Clean up the mock file
  std::remove(kTestFilePath.c_str());
}

TEST(LinuxParserTest, Jiffies) {
  // Create a mock /tmp/test_stat file for testing
  const std::string kTestFilePath = "/tmp/test_stat";
  std::ofstream test_file(kTestFilePath);
  test_file << "cpu  1059773 19667 216745 24069858 21792 0 2373 0 0 0\n";
  test_file << "cpu0 73808 1974 30694 2005066 1779 0 50 0 0 0\n";
  test_file << "cpu1 90791 2000 16144 2005259 2114 0 20 0 0 0\n";
  test_file.close();

  // Temporarily override the file path in LinuxParser
  const std::string original_stat_path = LinuxParser::kStatFilename;
  const_cast<std::string &>(LinuxParser::kStatFilename) = kTestFilePath;

  // Call the function and check the result
  long calculated_jiffies = LinuxParser::Jiffies();
  long expected_jiffies =
      1059773 + 19667 + 216745 + 24069858 + 21792 + 0 + 2373 + 0 + 0 + 0;
  EXPECT_GE(calculated_jiffies, 0);

  // Restore the original file path
  const_cast<std::string &>(LinuxParser::kStatFilename) = original_stat_path;

  // Clean up the mock file
  std::remove(kTestFilePath.c_str());
}




int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}