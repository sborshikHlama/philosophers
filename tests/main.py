import unittest
import subprocess
import time
import re

class TestPhilosophers(unittest.TestCase):
    """Test suite for the Philosophers problem."""

    def run_philo(self, args, timeout=5):
        """Runs the philo program with given arguments and captures output live."""
        try:
            process = subprocess.Popen(
                ["../philo"] + args,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
                universal_newlines=True
            )
            stdout_lines = []
            stderr_lines = []

            start_time = time.time()
            while time.time() - start_time < timeout:
                output = process.stdout.readline()
                if output:
                    stdout_lines.append(output)
                if process.poll() is not None:
                    break
            
            process.terminate()
            stdout, stderr = process.communicate()
            stdout_lines.append(stdout)
            stderr_lines.append(stderr)
            retcode = process.returncode
        except subprocess.TimeoutExpired:
            process.kill()
            stdout, stderr = process.communicate()
            retcode = -1
        finally:
            process.stdout.close()
            process.stderr.close()
        return retcode, "".join(stdout_lines), "".join(stderr_lines)  

    def test_basic_execution(self):
        """Test if philo runs successfully and no one dies in infinite mode."""
        retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200"], timeout=5)
        self.assertNotIn("died", stdout, "No philosopher should die.")
        self.assertTrue(re.search("is eating", stdout), "Philosophers should be eating with timestamp and ID.")
        self.assertEqual(stderr.strip(), "", f"Unexpected error output: {stderr}")

    def test_one_philosopher(self):
        """Test behavior with only one philosopher (should not eat)."""
        retcode, stdout, stderr = self.run_philo(["1", "800", "200", "200"])
        self.assertEqual(retcode, 0, "philo should run without errors")
        self.assertIn("has taken a fork", stdout, "One philosopher should try to take a fork")
        self.assertNotIn("is eating", stdout, "One philosopher cannot eat")
        self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")

    # def test_deadlock_prevention(self):
    #     """Test if program avoids deadlock by ensuring progress."""
    #     retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200"], timeout=10)
    #     if retcode != 0:
    #         print(f"STDERR OUTPUT:\n{stderr}")
    #         print(f"STDOUT OUTPUT:\n{stdout}")
    #     self.assertEqual(retcode, 0, "philo should run without errors")
    #     self.assertIn("is eating", stdout, "At least one philosopher should be eating")
    #     self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")

    def test_starvation_prevention(self):
        """Ensure no philosopher starves by checking that all eat at least once."""
        retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200", "5"])
        self.assertEqual(retcode, 0, "philo should run without errors")
        eat_counts = stdout.count("is eating")
        self.assertGreater(eat_counts, 4, "Each philosopher should eat at least once")
        self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")

    def test_timing(self):
        """Verify that the philosophers respect the given times for eating, sleeping, and thinking."""
        start = time.time()
        retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200", "3"])
        duration = time.time() - start
        self.assertGreaterEqual(duration, 1.2, "Simulation should last long enough")
        self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")


if __name__ == "__main__":
    unittest.main()
