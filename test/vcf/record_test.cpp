/**
 * Copyright 2017 EMBL - European Bioinformatics Institute
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory>

#include "catch/catch.hpp"

#include "vcf/file_structure.hpp"
#include "vcf/error.hpp"

namespace ebi
{
    
    TEST_CASE("Record constructor v41", "[constructor]")
    {
        std::shared_ptr<vcf::Source> source{
            new vcf::Source{
                "Example VCF source",
                vcf::InputFormat::VCF_FILE_VCF | vcf::InputFormat::VCF_FILE_BGZIP,
                vcf::Version::v41,
                vcf::Ploidy{2, {{"Y", 1}}},
                {},
                { "Sample1" }}};
            
        source->meta_entries.emplace("FORMAT",
            vcf::MetaEntry{
                1,
                "FORMAT",
                {
                    { "ID", "GT" },
                    { "Number", "1" },
                    { "Type", "String" },
                    { "Description", "Genotype" }
                },
                source
        });
           
        source->meta_entries.emplace("FORMAT",
            vcf::MetaEntry{
                1,
                "FORMAT",
                {
                    { "ID", "DP" },
                    { "Number", "1" },
                    { "Type", "Integer" },
                    { "Description", "Read depth" }
                },
                source
            });

        source->meta_entries.emplace("INFO",
            vcf::MetaEntry{
                1,
                "INFO",
                {
                    { "ID", "AN" },
                    { "Number", "1" },
                    { "Type", "Integer" },
                    { "Description", "Allele number" }
                },
                source
        });
           
        source->meta_entries.emplace("INFO",
            vcf::MetaEntry{
                1,
                "INFO",
                {
                    { "ID", "AF" },
                    { "Number", "A" },
                    { "Type", "Float" },
                    { "Description", "Allele frequency" }
                },
                source
            });

         
        SECTION("Correct arguments") 
        {
            CHECK_NOTHROW( (vcf::Record {
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "AC", "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source} ) );
                
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { }, 
                                "A", 
                                { "AC", "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}) );
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1",
                                123456,
                                { },
                                "A",
                                { "AC", "AT" },
                                1.0,
                                { "PASS" },
                                { {"AN", "12"}, {"AF", "0.5,0.3"} },
                                { "DP" },
                                { "1" },
                                source}) );
        }

        SECTION("Chromosome with whitespaces") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr 1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "AC", "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}),
                            vcf::ChromosomeBodyError*);
        }

        SECTION("Chromosome with colons") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr:1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "AC", "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}),
                            vcf::ChromosomeBodyError*);
        }

        SECTION("ID with whitespaces") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id 123", "id456" }, 
                                "A", 
                                { "AC", "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}),
                            vcf::IdBodyError*);
        }

        SECTION("Different length alleles")
        {
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}) );
                                
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "CT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}) );
        }
        
        SECTION("Same length alleles") 
        {
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1",
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "T", "C" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}) );
        }

        SECTION("Same alleles") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr1",
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "A" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}),
                            vcf::AlternateAllelesBodyError*);
        }

        SECTION("Less-than-zero quality") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "C" }, 
                                -1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}),
                            vcf::QualityBodyError*);
        }

        SECTION("Empty INFO") 
        {
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "T", "C" }, 
                                1.0, 
                                { "PASS" }, 
                                { { ".", "." } }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}) );
        }

        SECTION("Single-field format") 
        {
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "T", "C" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT" }, 
                                { "0|1" },
                                source}) );
                                
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "T", "C" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "DP" },
                                { "13" },
                                source}) );
        }
        
        SECTION("Multi-field format") 
        {
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "T", "C" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}) );
            
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A", 
                                { "T", "C" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "DP", "GL" }, 
                                { "12:0.5" },
                                source}) );
                                
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A",
                                { "T", "C" },
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "DP", "GT" }, 
                                { "12:0|1" },
                                source}),
                            vcf::FormatBodyError*);
        }

        SECTION("Unusual ploidy")
        {
            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "chr1",
                                123456,
                                { "id123", "id456" },
                                "A",
                                { "T", "C" },
                                1.0,
                                { "PASS" },
                                { {"AN", "12"}, {"AF", "0.5,0.3"} },
                                { "GT", "DP" },
                                { "0|1" },
                                source}) );

            CHECK_NOTHROW( (vcf::Record{
                                1,
                                "Y",
                                123456,
                                { "id123", "id456" },
                                "A",
                                { "T", "C" },
                                1.0,
                                { "PASS" },
                                { {"AN", "12"}, {"AF", "0.5,0.3"} },
                                { "GT", "DP" },
                                { "0" },
                                source}) );

// The next check is commented because a mismatch is currently only a warning, but we will process it as an error in the future
//            CHECK_THROWS_AS( (vcf::Record{
//                                1,
//                                "UnspecifiedTriploid",
//                                123456,
//                                { "id123", "id456" },
//                                "A",
//                                { "T", "C" },
//                                1.0,
//                                { "PASS" },
//                                { {"AN", "12"}, {"AF", "0.5,0.3"} },
//                                { "GT" },
//                                { "0|1|1" },
//                                source}),
//                            vcf::SamplesFieldBodyError*);
        }
    }

    TEST_CASE("Record constructor v43", "[constructor]")
    {            
        std::shared_ptr<vcf::Source> source{
            new vcf::Source{
                "Example VCF source",
                vcf::InputFormat::VCF_FILE_VCF | vcf::InputFormat::VCF_FILE_BGZIP,
                vcf::Version::v43,
                vcf::Ploidy{2, {{"Y", 1}}},
                {},
                { "Sample1" }}};

        source->meta_entries.emplace("FORMAT",
            vcf::MetaEntry{
                1,
                "FORMAT",
                {
                    { "ID", "GT" },
                    { "Number", "1" },
                    { "Type", "String" },
                    { "Description", "Genotype" }
                },
                source
        });
           
        source->meta_entries.emplace("FORMAT",
            vcf::MetaEntry{
                1,
                "FORMAT",
                {
                    { "ID", "DP" },
                    { "Number", "1" },
                    { "Type", "Integer" },
                    { "Description", "Read depth" }
                },
                source
            });

        source->meta_entries.emplace("INFO",
            vcf::MetaEntry{
                1,
                "INFO",
                {
                    { "ID", "AN" },
                    { "Number", "1" },
                    { "Type", "Integer" },
                    { "Description", "Allele number" }
                },
                source
        });
           
        source->meta_entries.emplace("INFO",
            vcf::MetaEntry{
                1,
                "INFO",
                {
                    { "ID", "AF" },
                    { "Number", "A" },
                    { "Type", "Float" },
                    { "Description", "Allele frequency" }
                },
                source
            });

        SECTION("Duplicate FORMATs") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id456" }, 
                                "A",
                                { "T", "C" },
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "DP", "DP" }, 
                                { "12:13" },
                                source}),
                            vcf::FormatBodyError*);
        }

        SECTION("Duplicate IDs") 
        {
            CHECK_THROWS_AS( (vcf::Record{
                                1,
                                "chr1", 
                                123456, 
                                { "id123", "id123" }, 
                                "A", 
                                { "AC", "AT" }, 
                                1.0, 
                                { "PASS" }, 
                                { {"AN", "12"}, {"AF", "0.5,0.3"} }, 
                                { "GT", "DP" }, 
                                { "0|1" },
                                source}),
                            vcf::IdBodyError*);
        }
    }
}
