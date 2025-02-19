import unittest
import subprocess
import time
import re

with open("no-die.txt", "r") as shouldnot_die:
	shouldnot_die_list = shouldnot_die.readlines()

with open("yes-die.txt", "r") as should_die:
	should_die_list = should_die.readlines()

with open ("crash_test.txt", "r") as crash_test:
    crash_list = crash_test.readlines()

with open ("meals_limits.txt", "r") as meals_limits:
    meals_limits_list = meals_limits.readlines()

def progress_bar(progress, total):
    percent = 100 * (progress / float(total))
    bar = '#' * percent + '-' * (100 - percent)
    print(f"\r|{bar}| {percent: .2f}%", end="\r")

class TestPhilosophers(unittest.TestCase):
    """Test suite for the Philosophers problem."""

    def run_philo(self, args, meals_to_eat=None, timeout=10):
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
            eat_logs = []
            start_time = time.time()

            while True:
                output = process.stdout.readline()
                if output:
                    stdout_lines.append(output.strip()) 
                    if "is eating" in output:
                        eat_logs.append(output)

                if meals_to_eat and len(eat_logs) // int(args[0]) == int(args[-1]):
                    print("Meal limit reached, stopping philo.")
                    process.terminate()
                    break

                if timeout and (time.time() - start_time) >= timeout:
                    print(f"Timeout reached: {timeout} seconds, terminating philo.")
                    process.terminate()
                    break

            stdout, stderr = process.communicate(timeout=5)  
            stdout_lines.append(stdout.strip())
            stderr_lines.append(stderr.strip())
            retcode = process.returncode
        except subprocess.TimeoutExpired:
            print("Process timeout expired, force killing.")
            process.kill()
            stdout, stderr = process.communicate()
            retcode = -1
        finally:
            if process.stdout:
                process.stdout.close()
            if process.stderr:
                process.stderr.close()

        return retcode, "\n".join(stdout_lines), "\n".join(stderr_lines)


    def test_basic_execution(self):
        """Test if philo runs successfully and no one dies in infinite mode."""
        print("Program should run")
        print("input: 5 800 200 200")
        retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200"])
        self.assertNotIn("died", stdout, "No philosopher should die.")
        self.assertTrue(re.search("is eating", stdout), "Philosophers should be eating with timestamp and ID.")
        self.assertEqual(stderr.strip(), "", f"Unexpected error output: {stderr}")
        print("PASSED")

    def test_no_die(self):
       for i in shouldnot_die_list:
           print("no one should die")
           print("input: " + i)
           retcode, stdout, stderr = self.run_philo(i.split(" "))
           self.assertNotIn("died", stdout, "No philosopher should die.")
           self.assertEqual(stderr.strip(), "", f"Unexpected error output: {stderr}")
           print("PASSED")

    def test_die(self):
       for i in should_die_list:
           print("one should die")
           print("input: " + i)
           retcode, stdout, stderr = self.run_philo(i.split(" "))
           self.assertIn("died", stdout, "A philosopher should die.")
           self.assertEqual(stderr.strip(), "", f"Unexpected error output: {stderr}")
           print("PASSED")
    
    # def test_crash(self):
    #     for i in crash_list:
    #         print("Programm should return error, no crash")
    #         print("input: " + i)
    #         retcode, stdout, stderr = self.run_philo(i.split(" "))
    #         self.assertNotIn("Error: only positive numbers allowed\n", stdout, "Error should be displayed")
    #         self.assertEqual(stderr.strip(), "", f"Unexpected error output: {stderr}")			 
    #         print("PASSED")
        
    def test_meals_limit(self):
        for i in meals_limits_list:
            meals_to_eat = i.split(" ")[-1]
            print("No one should die and philos must eat all meals")
            print("input: " + i)
            retcode, stdout, stderr = self.run_philo(i.split(" "), meals_to_eat=i.split(" ")[-1], timeout=None)
            eat_logs = re.findall(r"is eating", stdout)
            self.assertEqual((len(eat_logs) / int(i[0])), int(meals_to_eat), "Philosophers should eat all meals")
            self.assertNotIn("died", stdout, "No one should die")
            print("PASSED")
           
    def test_one_philosopher(self):
        """Test behavior with only one philosopher (should not eat)."""
        retcode, stdout, stderr = self.run_philo(["1", "800", "200", "200"])
        self.assertEqual(retcode, 0, "philo should run without errors")
        self.assertIn("has taken a fork", stdout, "One philosopher should try to take a fork")
        self.assertNotIn("is eating", stdout, "One philosopher cannot eat")
        self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")
        print("PASSED")

#     # def test_deadlock_prevention(self):
#     #     """Test if program avoids deadlock by ensuring progress."""
#     #     retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200"], timeout=10)
#     #     if retcode != 0:
#     #         print(f"STDERR OUTPUT:\n{stderr}")
#     #         print(f"STDOUT OUTPUT:\n{stdout}")
#     #     self.assertEqual(retcode, 0, "philo should run without errors")
#     #     self.assertIn("is eating", stdout, "At least one philosopher should be eating")
#     #     self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")

#     def test_starvation_prevention(self):
#         """Ensure no philosopher starves by checking that all eat at least once."""
#         retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200", "5"])
#         self.assertEqual(retcode, 0, "philo should run without errors")
#         eat_counts = stdout.count("is eating")
#         self.assertGreater(eat_counts, 4, "Each philosopher should eat at least once")
#         self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")

#     # def test_timing(self):
#     #     """Verify that the philosophers respect the given times for eating, sleeping, and thinking."""
#     #     start = time.time()
#     #     retcode, stdout, stderr = self.run_philo(["5", "800", "200", "200", "3"])
#     #     duration = time.time() - start
#     #     self.assertGreaterEqual(duration, 1.2, "Simulation should last long enough")
#     #     self.assertEqual(stderr, "", f"Unexpected error output: {stderr}")


if __name__ == "__main__":
    unittest.main()
