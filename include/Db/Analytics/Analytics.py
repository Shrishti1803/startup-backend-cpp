import csv
import matplotlib.pyplot as plt

months = []
deal_counts = []

with open("analytics.csv", "r") as file:
    reader = csv.DictReader(file)

    for row in reader:
        months.append(row["month"])
        deal_counts.append(int(row["total_deals"]))

plt.figure(figsize=(10, 5))

plt.plot(months, deal_counts, marker='o')

plt.title("Monthly Deals Analytics")
plt.xlabel("Month")
plt.ylabel("Number of Deals")

plt.grid(True)

plt.tight_layout()

plt.show()