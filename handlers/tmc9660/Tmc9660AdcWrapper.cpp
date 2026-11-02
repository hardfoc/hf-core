/**
 * @file Tmc9660AdcWrapper.cpp
 * @brief Implementation of the TMC9660 ADC delegation wrapper.
 *
 * @details
 * Every method is a one-line delegation to handler_.AdcInstance().Method().
 * No additional logic is performed in this layer.
 *
 * @see Tmc9660AdcWrapper  Class declaration and architectural documentation.
 * @see Tmc9660Handler::Adc  The actual ADC implementation being delegated to.
 *
 * @author HardFOC Team
 * @date 2025
 */

#include "Tmc9660AdcWrapper.h"
#include "Tmc9660Handler.h"

//==============================================================================
// CONSTRUCTION
//==============================================================================

Tmc9660AdcWrapper::Tmc9660AdcWrapper(Tmc9660Handler& handler) noexcept
    : handler_(handler) {
}

//==============================================================================
// BASE ADC DELEGATION
//==============================================================================

bool Tmc9660AdcWrapper::Initialize() noexcept {
    return handler_.AdcInstance().Initialize();
}

bool Tmc9660AdcWrapper::Deinitialize() noexcept {
    return handler_.AdcInstance().Deinitialize();
}

hf_u8_t Tmc9660AdcWrapper::GetMaxChannels() const noexcept {
    return handler_.AdcInstance().GetMaxChannels();
}

bool Tmc9660AdcWrapper::IsChannelAvailable(hf_channel_id_t channel_id) const noexcept {
    return handler_.AdcInstance().IsChannelAvailable(channel_id);
}

hf_adc_err_t Tmc9660AdcWrapper::ReadChannelV(hf_channel_id_t channel_id, float& channel_reading_v,
                                           hf_u8_t numOfSamplesToAvg,
                                           hf_time_t timeBetweenSamples) noexcept {
    return handler_.AdcInstance().ReadChannelV(channel_id, channel_reading_v,
                                       numOfSamplesToAvg, timeBetweenSamples);
}

hf_adc_err_t Tmc9660AdcWrapper::ReadChannelCount(hf_channel_id_t channel_id,
                                               hf_u32_t& channel_reading_count,
                                               hf_u8_t numOfSamplesToAvg,
                                               hf_time_t timeBetweenSamples) noexcept {
    return handler_.AdcInstance().ReadChannelCount(channel_id, channel_reading_count,
                                           numOfSamplesToAvg, timeBetweenSamples);
}

hf_adc_err_t Tmc9660AdcWrapper::ReadChannel(hf_channel_id_t channel_id,
                                         hf_u32_t& channel_reading_count,
                                         float& channel_reading_v,
                                         hf_u8_t numOfSamplesToAvg,
                                         hf_time_t timeBetweenSamples) noexcept {
    return handler_.AdcInstance().ReadChannel(channel_id, channel_reading_count, channel_reading_v,
                                      numOfSamplesToAvg, timeBetweenSamples);
}

hf_adc_err_t Tmc9660AdcWrapper::ReadMultipleChannels(const hf_channel_id_t* channel_ids,
                                                   hf_u8_t num_channels,
                                                   hf_u32_t* readings,
                                                   float* voltages) noexcept {
    return handler_.AdcInstance().ReadMultipleChannels(channel_ids, num_channels, readings, voltages);
}

hf_adc_err_t Tmc9660AdcWrapper::GetStatistics(hf_adc_statistics_t& statistics) noexcept {
    return handler_.AdcInstance().GetStatistics(statistics);
}

hf_adc_err_t Tmc9660AdcWrapper::GetDiagnostics(hf_adc_diagnostics_t& diagnostics) noexcept {
    return handler_.AdcInstance().GetDiagnostics(diagnostics);
}

hf_adc_err_t Tmc9660AdcWrapper::ResetStatistics() noexcept {
    return handler_.AdcInstance().ResetStatistics();
}

hf_adc_err_t Tmc9660AdcWrapper::ResetDiagnostics() noexcept {
    return handler_.AdcInstance().ResetDiagnostics();
}
