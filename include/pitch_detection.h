#ifndef PITCH_DETECTION_H
#define PITCH_DETECTION_H

#include <complex>
#include <ffts/ffts.h>
#include <stdexcept>
#include <vector>

/*
 * The pitch namespace contains the functions:
 *
 * 	pitch::mpm(data, sample_rate)
 * 	pitch::yin(data, sample_rate)
 *
 * It will auto-allocate any buffers.
 */
namespace pitch
{

double
yin(const std::vector<double> &, int);

double
mpm(const std::vector<double> &, int);

} // namespace pitch

/*
 * The pitch_alloc namespace contains the functions:
 *
 * 	pitch_alloc::mpm(data, sample_rate, pitch_alloc::Mpm)
 * 	pitch_alloc::yin(data, sample_rate, pitch_alloc::Yin)
 *
 * It also contains the classes Yin and Mpm which contain the buffers.
 *
 * This namespace is used for Bring-Your-Own-Alloc.
 */
namespace pitch_alloc
{

/*
 * Allocate the buffers for MPM for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: pitch_alloc::Mpm ma(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<1)
 */
class Mpm
{
  public:
	long N;
	std::vector<std::complex<float>> out_im;
	std::vector<double> out_real;
	ffts_plan_t *fft_forward;
	ffts_plan_t *fft_backward;

	Mpm(long audio_buffer_size)
	    : N(audio_buffer_size), out_im(std::vector<std::complex<float>>(N * 2)),
	      out_real(std::vector<double>(N))
	{
		if (N == 0) {
			throw std::bad_alloc();
		}

		fft_forward = ffts_init_1d(N * 2, FFTS_FORWARD);
		fft_backward = ffts_init_1d(N * 2, FFTS_BACKWARD);
	}

	~Mpm()
	{
		ffts_free(fft_forward);
		ffts_free(fft_backward);
	}

	void
	clear()
	{
		std::fill(out_im.begin(), out_im.end(), std::complex<double>(0.0, 0.0));
	}
};

/*
 * Allocate the buffers for YIN for re-use.
 * Intended for multiple consistently-sized audio buffers.
 *
 * Usage: pitch_alloc::Yin ya(1024)
 *
 * It will throw std::bad_alloc for invalid sizes (<2)
 */
class Yin : public Mpm
{
  public:
	std::vector<double> yin_buffer;

	Yin(long audio_buffer_size)
	    : Mpm(audio_buffer_size), yin_buffer(std::vector<double>(N / 2))
	{
		if (N / 2 == 0) {
			throw std::bad_alloc();
		}
	}
};

double
yin(const std::vector<double> &, int, Yin *);

double
mpm(const std::vector<double> &, int, Mpm *);
} // namespace pitch_alloc

#endif /* PITCH_DETECTION_H */
