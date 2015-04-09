/*
 * Copyright (c) 2008-2011 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/* We use the hw_value as an index into our private channel structure */

#include "common.h"

#define CHAN2G(_freq, _idx)  { \
	.band = IEEE80211_BAND_2GHZ, \
	.center_freq = (_freq), \
	.hw_value = (_idx), \
	.max_power = 33, \
}

#define CHAN5G(_freq, _idx) { \
	.band = IEEE80211_BAND_5GHZ, \
	.center_freq = (_freq), \
	.hw_value = (_idx), \
	.max_power = 33, \
}

/* Some 2 GHz radios are actually tunable on 2312-2732
 * on 5 MHz steps, we support the channels which we know
 * we have calibration data for all cards though to make
 * this static */
static const struct ieee80211_channel ath9k_2ghz_chantable[] = {
	CHAN2G(2407, 0), /* Channel 0 */
	CHAN2G(2412, 1), /* Channel 1 */
	CHAN2G(2417, 2), /* Channel 2 */
	CHAN2G(2422, 3), /* Channel 3 */
	CHAN2G(2427, 4), /* Channel 4 */
	CHAN2G(2432, 6), /* Channel 5 */
	CHAN2G(2437, 6), /* Channel 6 */
	CHAN2G(2442, 7), /* Channel 7 */
	CHAN2G(2447, 8), /* Channel 8 */
	CHAN2G(2452, 9), /* Channel 9 */
	CHAN2G(2457, 10), /* Channel 10 */
	CHAN2G(2462, 11), /* Channel 11 */
	CHAN2G(2467, 12), /* Channel 12 */
	CHAN2G(2472, 13), /* Channel 13 */
	CHAN2G(2484, 14), /* Channel 14 */
	CHAN2G(2312, 237), /* Channel -19 */
	CHAN2G(2317, 238), /* Channel -18 */
	CHAN2G(2322, 239), /* Channel -17 */
	CHAN2G(2327, 240), /* Channel -16 */
	CHAN2G(2332, 241), /* Channel -15 */
	CHAN2G(2337, 242), /* Channel -14 */
	CHAN2G(2342, 243), /* Channel -13 */
	CHAN2G(2347, 244), /* Channel -12 */
	CHAN2G(2352, 245), /* Channel -11 */
	CHAN2G(2357, 246), /* Channel -10 */
	CHAN2G(2362, 247), /* Channel -9 */
	CHAN2G(2367, 248), /* Channel -8 */
	CHAN2G(2372, 249), /* Channel -7 */
	CHAN2G(2377, 250), /* Channel -6 */
	CHAN2G(2382, 251), /* Channel -5 */
	CHAN2G(2387, 252), /* Channel -4 */
	CHAN2G(2392, 253), /* Channel -3 */
	CHAN2G(2397, 254), /* Channel -2 */
	CHAN2G(2402, 255), /* Channel -1 */

};

/* Some 5 GHz radios are actually tunable on XXXX-YYYY
 * on 5 MHz steps, we support the channels which we know
 * we have calibration data for all cards though to make
 * this static */
static const struct ieee80211_channel ath9k_5ghz_chantable[] = {
	/* _We_ call this UNII 1 */
	CHAN5G(5180, 15), /* Channel 36 */
	CHAN5G(5200, 16), /* Channel 40 */
	CHAN5G(5220, 17), /* Channel 44 */
	CHAN5G(5240, 18), /* Channel 48 */
	/* _We_ call this UNII 2 */
	CHAN5G(5260, 19), /* Channel 52 */
	CHAN5G(5280, 20), /* Channel 56 */
	CHAN5G(5300, 21), /* Channel 60 */
	CHAN5G(5320, 22), /* Channel 64 */
	/* _We_ call this "Middle band" */
	CHAN5G(5500, 23), /* Channel 100 */
	CHAN5G(5520, 24), /* Channel 104 */
	CHAN5G(5540, 25), /* Channel 108 */
	CHAN5G(5560, 26), /* Channel 112 */
	CHAN5G(5580, 27), /* Channel 116 */
	CHAN5G(5600, 28), /* Channel 120 */
	CHAN5G(5620, 29), /* Channel 124 */
	CHAN5G(5640, 30), /* Channel 128 */
	CHAN5G(5660, 31), /* Channel 132 */
	CHAN5G(5675, 32), /* Channel 135 */
	CHAN5G(5680, 33), /* Channel 136 */
	CHAN5G(5685, 34), /* Channel 137 */
	CHAN5G(5695, 35), /* Channel 139 */
	CHAN5G(5700, 36), /* Channel 140 */
	/* _We_ call this UNII 3 */
	CHAN5G(5745, 37), /* Channel 149 */
	CHAN5G(5765, 38), /* Channel 153 */
	CHAN5G(5785, 39), /* Channel 157 */
	CHAN5G(5795, 40), /* Channel 159 */
	CHAN5G(5805, 41), /* Channel 161 */
	CHAN5G(5815, 42), /* Channel 163 */
	CHAN5G(5825, 43), /* Channel 165 */
};

/* Atheros hardware rate code addition for short premble */
#define SHPCHECK(__hw_rate, __flags) \
	((__flags & IEEE80211_RATE_SHORT_PREAMBLE) ? (__hw_rate | 0x04 ) : 0)

#define RATE(_bitrate, _hw_rate, _flags) {              \
	.bitrate        = (_bitrate),                   \
	.flags          = (_flags),                     \
	.hw_value       = (_hw_rate),                   \
	.hw_value_short = (SHPCHECK(_hw_rate, _flags))  \
}

static struct ieee80211_rate ath9k_legacy_rates[] = {
	RATE(10, 0x1b, 0),
	RATE(20, 0x1a, IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(55, 0x19, IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(110, 0x18, IEEE80211_RATE_SHORT_PREAMBLE),
	RATE(60, 0x0b, (IEEE80211_RATE_SUPPORTS_5MHZ |
			IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(90, 0x0f, (IEEE80211_RATE_SUPPORTS_5MHZ |
			IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(120, 0x0a, (IEEE80211_RATE_SUPPORTS_5MHZ |
			 IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(180, 0x0e, (IEEE80211_RATE_SUPPORTS_5MHZ |
			 IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(240, 0x09, (IEEE80211_RATE_SUPPORTS_5MHZ |
			 IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(360, 0x0d, (IEEE80211_RATE_SUPPORTS_5MHZ |
			 IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(480, 0x08, (IEEE80211_RATE_SUPPORTS_5MHZ |
			 IEEE80211_RATE_SUPPORTS_10MHZ)),
	RATE(540, 0x0c, (IEEE80211_RATE_SUPPORTS_5MHZ |
			 IEEE80211_RATE_SUPPORTS_10MHZ)),
};

int ath9k_cmn_init_channels_rates(struct ath_common *common)
{
	struct ath_hw *ah = (struct ath_hw *)common->ah;
	void *channels;

	BUILD_BUG_ON(ARRAY_SIZE(ath9k_2ghz_chantable) +
		     ARRAY_SIZE(ath9k_5ghz_chantable) !=
		     ATH9K_NUM_CHANNELS);

	if (ah->caps.hw_caps & ATH9K_HW_CAP_2GHZ) {
		channels = devm_kzalloc(ah->dev,
			sizeof(ath9k_2ghz_chantable), GFP_KERNEL);
		if (!channels)
		    return -ENOMEM;

		memcpy(channels, ath9k_2ghz_chantable,
		       sizeof(ath9k_2ghz_chantable));
		common->sbands[IEEE80211_BAND_2GHZ].channels = channels;
		common->sbands[IEEE80211_BAND_2GHZ].band = IEEE80211_BAND_2GHZ;
		common->sbands[IEEE80211_BAND_2GHZ].n_channels =
			ARRAY_SIZE(ath9k_2ghz_chantable);
		common->sbands[IEEE80211_BAND_2GHZ].bitrates = ath9k_legacy_rates;
		common->sbands[IEEE80211_BAND_2GHZ].n_bitrates =
			ARRAY_SIZE(ath9k_legacy_rates);
	}

	if (ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ) {
		channels = devm_kzalloc(ah->dev,
			sizeof(ath9k_5ghz_chantable), GFP_KERNEL);
		if (!channels)
			return -ENOMEM;

		memcpy(channels, ath9k_5ghz_chantable,
		       sizeof(ath9k_5ghz_chantable));
		common->sbands[IEEE80211_BAND_5GHZ].channels = channels;
		common->sbands[IEEE80211_BAND_5GHZ].band = IEEE80211_BAND_5GHZ;
		common->sbands[IEEE80211_BAND_5GHZ].n_channels =
			ARRAY_SIZE(ath9k_5ghz_chantable);
		common->sbands[IEEE80211_BAND_5GHZ].bitrates =
			ath9k_legacy_rates + 4;
		common->sbands[IEEE80211_BAND_5GHZ].n_bitrates =
			ARRAY_SIZE(ath9k_legacy_rates) - 4;
	}
	return 0;
}
EXPORT_SYMBOL(ath9k_cmn_init_channels_rates);

void ath9k_cmn_setup_ht_cap(struct ath_hw *ah,
			    struct ieee80211_sta_ht_cap *ht_info)
{
	struct ath_common *common = ath9k_hw_common(ah);
	u8 tx_streams, rx_streams;
	int i, max_streams;

	ht_info->ht_supported = true;
	ht_info->cap = IEEE80211_HT_CAP_SUP_WIDTH_20_40 |
		       IEEE80211_HT_CAP_SM_PS |
		       IEEE80211_HT_CAP_SGI_40 |
		       IEEE80211_HT_CAP_DSSSCCK40;

	if (ah->caps.hw_caps & ATH9K_HW_CAP_LDPC)
		ht_info->cap |= IEEE80211_HT_CAP_LDPC_CODING;

	if (ah->caps.hw_caps & ATH9K_HW_CAP_SGI_20)
		ht_info->cap |= IEEE80211_HT_CAP_SGI_20;

	ht_info->ampdu_factor = IEEE80211_HT_MAX_AMPDU_64K;
	ht_info->ampdu_density = IEEE80211_HT_MPDU_DENSITY_8;

	if (AR_SREV_9271(ah) || AR_SREV_9330(ah) || AR_SREV_9485(ah) || AR_SREV_9565(ah))
		max_streams = 1;
	else if (AR_SREV_9462(ah))
		max_streams = 2;
	else if (AR_SREV_9300_20_OR_LATER(ah))
		max_streams = 3;
	else
		max_streams = 2;

	if (AR_SREV_9280_20_OR_LATER(ah)) {
		if (max_streams >= 2)
			ht_info->cap |= IEEE80211_HT_CAP_TX_STBC;
		ht_info->cap |= (1 << IEEE80211_HT_CAP_RX_STBC_SHIFT);
	}

	/* set up supported mcs set */
	memset(&ht_info->mcs, 0, sizeof(ht_info->mcs));
	tx_streams = ath9k_cmn_count_streams(ah->txchainmask, max_streams);
	rx_streams = ath9k_cmn_count_streams(ah->rxchainmask, max_streams);

	ath_dbg(common, CONFIG, "TX streams %d, RX streams: %d\n",
		tx_streams, rx_streams);

	if (tx_streams != rx_streams) {
		ht_info->mcs.tx_params |= IEEE80211_HT_MCS_TX_RX_DIFF;
		ht_info->mcs.tx_params |= ((tx_streams - 1) <<
				IEEE80211_HT_MCS_TX_MAX_STREAMS_SHIFT);
	}

	for (i = 0; i < rx_streams; i++)
		ht_info->mcs.rx_mask[i] = 0xff;

	ht_info->mcs.tx_params |= IEEE80211_HT_MCS_TX_DEFINED;
}
EXPORT_SYMBOL(ath9k_cmn_setup_ht_cap);

void ath9k_cmn_reload_chainmask(struct ath_hw *ah)
{
	struct ath_common *common = ath9k_hw_common(ah);

	if (!(ah->caps.hw_caps & ATH9K_HW_CAP_HT))
		return;

	if (ah->caps.hw_caps & ATH9K_HW_CAP_2GHZ)
		ath9k_cmn_setup_ht_cap(ah,
			&common->sbands[IEEE80211_BAND_2GHZ].ht_cap);
	if (ah->caps.hw_caps & ATH9K_HW_CAP_5GHZ)
		ath9k_cmn_setup_ht_cap(ah,
			&common->sbands[IEEE80211_BAND_5GHZ].ht_cap);
}
EXPORT_SYMBOL(ath9k_cmn_reload_chainmask);
