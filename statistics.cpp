#include <iostream>
#include <limits>
#include <vector>

class IStatistics {
	public:
		virtual ~IStatistics() = default;
		virtual void update(const double& next) = 0;
		virtual double eval() const = 0;
		virtual const char * name() const = 0;
};

class Min : public IStatistics {
	public:
		Min() : m_min{std::numeric_limits<double>::max()} {}

		void update(const double& next) override {
			if (next < m_min) {
				m_min = next;
			}
		}

		double eval() const override {
			return m_min;
		}

		const char * name() const override {
			return "min";
		}
	private:
		double m_min;
};

class Max : public IStatistics {
	public:
		Max() : m_max{std::numeric_limits<double>::min()} {}

		void update(const double& next) override {
			if (next > m_max) {
				m_max = next;
			}
		}

		double eval() const override {
			return m_max;
		}

		const char * name() const override {
			return "max";
		}
	private:
		double m_max;
};

class Mean : public IStatistics {
	public:
		void update(const double& next) override {
			numbers.push_back(next);

			cached = false;
		}

		double eval() const override {
			if (cached) {
				return mean;
			}

			double sum = 0;

			for (auto &number: numbers) {
				sum += number;
			}

			mean = sum / static_cast<int>(numbers.size());

			cached = true;

			return mean;
		}

		const char * name() const override {
			return "mean";
		}
	private:
		std::vector<double> numbers{};
		mutable bool cached = false;
		mutable double mean = 0;
};

int main() {
	const size_t statistics_count = 3;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Max{};
	statistics[2] = new Mean{};

	double val = 0;

	while (std::cin >> val) {
		for (auto& statistic : statistics) {
			statistic->update(val);
		}
	}

	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";

		return 1;
	}

	for (auto& statistic : statistics) {
		std::cout << statistic->name() << " = " << statistic->eval() << std::endl;
	}

	for (auto& statistic : statistics) {
		delete statistic;
	}

	return 0;
}