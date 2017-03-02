/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include "fast/schema.hpp"
#include "fast/data_parser.hpp"
#include "fast/pmap_decoder.hpp"
#include "fast/field.hpp"
#include "fast/iostream.hpp"

std::vector< char > load_packet(const char* path);

int main(int argc, char* argv[])
{
    try {
        /* Load schema */
        fast::schema schema{"FIX50SP2-2017-Mar.xml"};

        /* Load sample packet */
        auto packet = load_packet("packet.data");

        /* Create parser */
        fast::data_parser parser{packet.data(), packet.data() + packet.size()};

        /* Create PMAP handler */
        fast::pmap_decoder pmap;

        /* Read PMAP */
        pmap.load(parser);

        std::uint32_t template_id;

        /* Check if PMAP bit set */
        if (pmap.is_bit_set()) {
            /* Decode template ID */
            parser.parse(template_id);
            std::cout << "Packet template id " << template_id << '\n';
            pmap.next();
        } else {
            assert( false );
        }

        assert( template_id == 2115 );

		//<string name="MessageType" id="35"><constant value="d" /></string>
		//<string name="ApplVerID" id="1128"><constant value="9"/></string>
		//<string name="BeginString" id="8"><constant value="FIXT.1.1"/></string>
		//<string name="SenderCompID" id="49"><constant value="MOEX"/></string>
		//<uInt32 name="MsgSeqNum" id="34"><increment/></uInt32>
		//<uInt64 name="SendingTime" id="52"></uInt64>
		//<string name="MessageEncoding" id="347"><default value="UTF-8"/></string>
		//<int32 name="TotNumReports" id="911" presence="optional"></int32>
		//<string name="Symbol" id="55" presence="optional"></string>
        //<byteVector name="SecurityID" id="48" presence="optional"></byteVector>
		//<byteVector name="SecurityIDSource" id="22" presence="optional"></byteVector>
		//<int32 name="Product" id="460" presence="optional"></int32>
		//<byteVector name="CFICode" id="461" presence="optional"></byteVector>
		//<byteVector name="SecurityType" id="167" presence="optional"></byteVector>
		//<uInt32 name="MaturityDate" id="541" presence="optional"></uInt32>
		//<uInt32 name="SettlDate" id="64" presence="optional"></uInt32>
		//<string name="SettleType" id="5459" presence="optional"></string>
		//<decimal name="OrigIssueAmt" id="5850" presence="optional"></decimal>
		//<uInt32 name="CouponPaymentDate" id="224" presence="optional"></uInt32>
		//<decimal name="CouponRate" id="223" presence="optional"></decimal>
		//<uInt32 name="SettlFixingDate" id="9119" presence="optional"></uInt32>
		//<decimal name="DividendNetPx" id="9982" presence="optional"></decimal>
		//<byteVector name="SecurityDesc" id="107" presence="optional"></byteVector>
		//<byteVector name="EncodedSecurityDesc" id="351" presence="optional"></byteVector>
		//<byteVector name="QuoteText" id="9696" presence="optional"></byteVector>

        /* Construct fields */
        fast::field< fast::ascii_string_cref, fast::op_constant, fast::presence_mandatory >
            MessageType{"MessageType", 35, fast::ascii_string_cref{"d"}};
        fast::field< fast::ascii_string_cref, fast::op_constant, fast::presence_mandatory >
            ApplVerID{"ApplVerID", 1128, fast::ascii_string_cref{"9"}};
        fast::field< fast::ascii_string_cref, fast::op_constant, fast::presence_mandatory >
            BeginString{"BeginString", 8, fast::ascii_string_cref{"FIXT.1.1"}};
        fast::field< fast::ascii_string_cref, fast::op_constant, fast::presence_mandatory >
            SenderCompID{"SenderCompID", 49, fast::ascii_string_cref{"MOEX"}};
        fast::field< std::uint32_t, fast::op_increment, fast::presence_mandatory >
            MsgSeqNum{"MsgSeqNum", 34};
        fast::field< std::uint64_t, fast::op_none, fast::presence_mandatory >
            SendingTime{"SendingTime", 52};
        fast::field< fast::ascii_string_cref, fast::op_default, fast::presence_mandatory >
            MessageEncoding{"MessageEncoding", 347, fast::ascii_string_cref{"UTF-8"}};
        fast::field< std::int32_t, fast::op_none, fast::presence_optional >
            TotNumReports{"TotNumReports", 911};
        fast::field< fast::ascii_string_cref, fast::op_none, fast::presence_optional >
            Symbol{"Symbol", 55};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            SecurityID{"SecurityID", 48};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            SecurityIDSource{"SecurityIDSource", 22};
        fast::field< std::int32_t, fast::op_none, fast::presence_optional >
            Product{"Product", 460};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            CFICode{"CFICode", 461};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            SecurityType{"SecurityType", 167};
        fast::field< std::uint32_t, fast::op_none, fast::presence_optional >
            MaturityDate{"MaturityDate", 541};
        fast::field< std::uint32_t, fast::op_none, fast::presence_optional >
            SettlDate{"SettlDate", 64};
        fast::field< fast::ascii_string_cref, fast::op_none, fast::presence_optional >
            SettleType{"SettleType", 5459};
        fast::field< fast::decimal, fast::op_none, fast::presence_optional >
            OrigIssueAmt{"OrigIssueAmt", 5850};
        fast::field< std::uint32_t, fast::op_none, fast::presence_optional >
            CouponPaymentDate{"CouponPaymentDate", 224};
        fast::field< fast::decimal, fast::op_none, fast::presence_optional >
            CouponRate{"CouponRate", 223};
        fast::field< std::uint32_t, fast::op_none, fast::presence_optional >
            SettlFixingDate{"SettlFixingDate", 9119};
        fast::field< fast::decimal, fast::op_none, fast::presence_optional >
            DividendNetPx{"DividendNetPx", 9982};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            SecurityDesc{"SecurityDesc", 107};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            EncodedSecurityDesc{"EncodedSecurityDesc", 351};
        fast::field< fast::vector_cref, fast::op_none, fast::presence_optional >
            QuoteText{"QuoteText", 9696};

        MessageType.decode(parser, pmap);
        ApplVerID.decode(parser, pmap);
        BeginString.decode(parser, pmap);
        SenderCompID.decode(parser, pmap);
        MsgSeqNum.decode(parser, pmap);
        SendingTime.decode(parser, pmap);
        MessageEncoding.decode(parser, pmap);
        TotNumReports.decode(parser, pmap);
        Symbol.decode(parser, pmap);
        SecurityID.decode(parser, pmap);
        SecurityIDSource.decode(parser, pmap);
        Product.decode(parser, pmap);
        CFICode.decode(parser, pmap);
        SecurityType.decode(parser, pmap);
        MaturityDate.decode(parser, pmap);
        SettlDate.decode(parser, pmap);
        SettleType.decode(parser, pmap);
        OrigIssueAmt.decode(parser, pmap);
        CouponPaymentDate.decode(parser, pmap);
        CouponRate.decode(parser, pmap);
        SettlFixingDate.decode(parser, pmap);
        DividendNetPx.decode(parser, pmap);
        SecurityDesc.decode(parser, pmap);
        EncodedSecurityDesc.decode(parser, pmap);
        QuoteText.decode(parser, pmap);

        //<uInt32 name="SettlFixingDate" id="9119" presence="optional"></uInt32>
		//<decimal name="DividendNetPx" id="9982" presence="optional"></decimal>
		//<byteVector name="SecurityDesc" id="107" presence="optional"></byteVector>
		//<byteVector name="EncodedSecurityDesc" id="351" presence="optional"></byteVector>
		//<byteVector name="QuoteText" id="9696" presence="optional"></byteVector>

        std::cout << "DECODED\n";
        std::cout << MessageType << "\n";
        std::cout << ApplVerID << "\n";
        std::cout << BeginString << "\n";
        std::cout << SenderCompID << "\n";
        std::cout << MsgSeqNum << "\n";
        std::cout << SendingTime << "\n";
        std::cout << MessageEncoding << "\n";
        std::cout << TotNumReports << "\n";
        std::cout << Symbol << "\n";
        std::cout << SecurityID << "\n";
        std::cout << SecurityIDSource << "\n";
        std::cout << Product << "\n";
        std::cout << CFICode << "\n";
        std::cout << SecurityType << "\n";
        std::cout << MaturityDate << '\n';
        std::cout << SettlDate << '\n';
        std::cout << SettleType << '\n';
        std::cout << OrigIssueAmt << '\n';
        std::cout << CouponPaymentDate << '\n';
        std::cout << CouponRate << '\n';
        std::cout << SettlFixingDate << '\n';
        std::cout << DividendNetPx << '\n';
        std::cout << SecurityDesc << '\n';
        std::cout << EncodedSecurityDesc << '\n';
        std::cout << QuoteText << '\n';

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

std::vector< char > load_packet(const char* path)
{
    using namespace std::string_literals;

    std::ifstream file(path, std::ios::binary | std::ios::ate);
    auto size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector< char > result;
    result.resize(size);

    if (!file.read(result.data(), size)) {
        throw std::runtime_error("Failed to read packet file "s + path);
    }

    return std::move(result);
}
